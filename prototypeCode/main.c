/*
 * This program takes commands from a python script and sends back impedance data
 * over usb
 *
 */

// standard includes
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "nrf.h"
#include "nrf_drv_clock.h"
#include "nrf_gpio.h"
#include "nrf_drv_power.h"
#include "app_error.h"
#include "app_util.h"
#include "app_timer.h"

#include "nrf_delay.h"
#include "boards.h"
#include "app_util_platform.h"
#include "nrf_drv_twi.h"

#include "mem_manager.h"

#include "fds.h"

// logging includes
#ifdef DEBUG_LOG
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#endif

// my modules include
#include "AD5933.h"
#include "flashManager.h"
#include "usbManager.h"

// --- User Defines ---

bool recieveSweep(Sweep * sweep);
void set_default(Sweep * sweep);
bool sendSweep(uint32_t sweepNum);

// --- TWI Defines ---

// Needed to get the instance ID
#define TWI_INSTANCE_ID     0

// function declarations
void twi_handler(nrf_drv_twi_evt_t const * p_event, void * p_context);
void twi_init (void);


// get the TWI instance
const nrf_drv_twi_t m_twi = NRF_DRV_TWI_INSTANCE(TWI_INSTANCE_ID);

// Indicates if operation on TWI has ended
volatile bool m_xfer_done = false;

// Indicates TWI error
volatile bool twi_error = false;

// Main function
int main(void)
{
  bool i2c_stats;						// stores if i2c success
  uint8_t AD5933_status;		// to store the status
  ret_code_t ret;						// NRF status
	
  // init LEDS
  bsp_board_init(BSP_INIT_LEDS);

  // init Log
#ifdef DEBUG_LOG
  APP_ERROR_CHECK(NRF_LOG_INIT(NULL));
  NRF_LOG_DEFAULT_BACKENDS_INIT();
  NRF_LOG_INFO("Wireless Sensor Started");
  NRF_LOG_FLUSH();
#endif

  // init twi
  twi_init();

  // init USB
  usbManager_init();
	
	// init flashManager
	flashManager_init();
	
	// init memory manager
	nrf_mem_init();

  // reset the AD5933
  i2c_stats = AD5933_SetControl(NO_OPERATION, RANGE1, GAIN1, INTERN_CLOCK, 1);
  
	// create a new sweep
	Sweep sweep = {0};

  // set the sweep to default parameters
  set_default(&sweep);
	
	// variable to store the number of saved sweeps
	uint32_t numSweeps = 0;
	
	// load the config files from flash
	flashManager_checkConfig(&numSweeps, &sweep);

#ifdef DEBUG_LOG
  if (i2c_stats) {NRF_LOG_INFO("AD5933 Connected");}
  else {NRF_LOG_INFO("AD5933 Connection Failed");}
  NRF_LOG_FLUSH();
#endif

	// pointers to store sweep data
	uint32_t * freq;
	uint16_t * real;
	uint16_t * imag;
	
	uint8_t command[1]; // store the command from usb
	
  while (true)
  {
		// check if usb data is available, if it is get the first command
		if (usbManager_readReady() && usbManager_getByte(command))
		{
			// read the config file
			// send the number of saved sweeps
			if (command[0] == 1)
			{
				flashManager_checkConfig(&numSweeps, &sweep);
				usbManager_writeBytes(&numSweeps, sizeof(numSweeps));
			}
			// execute a sweep and save to flash
			else if (command[0] == 2)
			{
				// allocate memory for sweep data
				freq = nrf_malloc(sizeof(uint32_t) * (sweep.metadata.numPoints));
				real = nrf_malloc(sizeof(uint16_t) * (sweep.metadata.numPoints));
				imag = nrf_malloc(sizeof(uint16_t) * (sweep.metadata.numPoints));
				
				uint8_t buff[1]; // to save the result

				if (AD5933_Sweep(&sweep, freq, real, imag))
				{
					if (flashManager_saveSweep(freq, real, imag, &sweep.metadata, numSweeps + 1))
					{
						numSweeps += 1;
						flashManager_updateNumSweeps(&numSweeps);
						NRF_LOG_INFO("Sweep %d saved", numSweeps);
						NRF_LOG_FLUSH();
						
						// send sweep success over usb
						buff[0] = 2;
					}
					else
					{
						buff[0] = 1;
					}
				}
				else
				{
					buff[0] = 1;
				}
				// send result
				usbManager_writeBytes(buff, 1);
				
				// free the memory
				nrf_free(freq);
				nrf_free(real);
				nrf_free(imag);
			}
			// send the most recent sweep over usb
			else if (command[0] == 3)
			{
				// send back 3
				uint8_t buff[1] = {3};
				usbManager_writeBytes(buff, 1);
				
				// send the most recent sweep
				if ((numSweeps > 0) && sendSweep(numSweeps))
				{
					NRF_LOG_INFO("Sweep send success");
					numSweeps -= 1;
				}
				else
				{
					NRF_LOG_INFO("Sweep send fail");
				}
				NRF_LOG_FLUSH();
			}
    }
    // Sleep CPU only if there was no interrupt since last loop processing
    __WFE();
	}
}

bool sendSweep(uint32_t sweepNum)
{
	// allocate memory for sweeps
	uint32_t * freq = nrf_malloc(MAX_FREQ_SIZE);
	uint16_t * real = nrf_malloc(MAX_IMP_SIZE);
	uint16_t * imag = nrf_malloc(MAX_IMP_SIZE);
	
	// check if malloc fail
	if (freq == NULL || real == NULL || imag == NULL)
	{
#ifdef DEBUG_LOG
		NRF_LOG_INFO("Memory allocation failed. Send sweep fail");
		NRF_LOG_FLUSH();
#endif
		// free any memory that was successful
		nrf_free(freq);
		nrf_free(real);
		nrf_free(imag);
		return false;
	}
	
	MetaData metadata;		 // create a metadata to store current sweep data
	uint8_t buff[8];		  // buffer to store data points
	uint8_t * sel;			 //	pointer to select each byte in data
	bool ret = false;		// saves if get sweep fails
	
	// get sweep and send it over USB
	if(flashManager_getSweep(freq, real, imag, &metadata, sweepNum))
	{
		// loop through each point in the sweep and send it over usb
		for (int i = 0; i < metadata.numPoints; i++)
		{
			// cut up currentFrequency into bytes
			sel = (uint8_t *) &freq[i];
			buff[0] = sel[0];
			buff[1] = sel[1];
			buff[2] = sel[2];
			buff[3] = sel[3];

			// copy the real and imaginary impedance values
			sel = (uint8_t *) &real[i];
			buff[4] = sel[0];
			buff[5] = sel[1];
			
			sel = (uint8_t *) &imag[i];
			buff[6] = sel[0];
			buff[7] = sel[1];

			// send all the data over usb
			usbManager_writeBytes(buff, 8);
		}
		ret = true;
	}
	
	// send a blank sweep to indicate sweep done
	uint8_t blank[8] = {0};
	usbManager_writeBytes(blank, 8);
	
	// free the memory
	nrf_free(freq);
	nrf_free(real);
	nrf_free(imag);
	
	// return if the sweep send successs
	return ret;
}

// recieves usb data for a sweep parameter over usb
bool recieveSweep(Sweep * sweep)
{
  uint8_t params[16];

  usbManager_readBytes(params, 16);

  return true;
}

void set_default(Sweep * sweep)
{
  // set the default sweep parameters
  sweep->start 							= 1000;
  sweep->delta 							= 100;
  sweep->steps 							= 490;
  sweep->cycles 						= 15;
  sweep->cyclesMultiplier 	= NO_MULT;
  sweep->range 							= RANGE1;
  sweep->clockSource 				= INTERN_CLOCK;
  sweep->clockFrequency 		= CLK_FREQ;
  sweep->gain 							= GAIN1;
	sweep->metadata.numPoints = sweep->steps + 1;
	sweep->metadata.temp			= 100;
	sweep->metadata.time			= 30;

  return;
}

// --- TWI Functions ---

// TWI events handler.
void twi_handler(nrf_drv_twi_evt_t const * p_event, void * p_context)
{
  switch (p_event -> type)
  {
    case NRF_DRV_TWI_EVT_DONE:
      if (p_event -> xfer_desc.type == NRF_DRV_TWI_XFER_RX)
      {
#if defined(DEBUG_LOG) && defined(DEBUG_TWI)
        NRF_LOG_INFO("TWI Read Success");
        NRF_LOG_FLUSH();
#endif
      }
      else if (p_event -> xfer_desc.type == NRF_DRV_TWI_XFER_TX)
      {
#if defined(DEBUG_LOG) && defined(DEBUG_TWI)
        NRF_LOG_INFO("TWI Write Success");
        NRF_LOG_FLUSH();
#endif
      }

      // set transfer done to true
      m_xfer_done = true;
      // set error to false
      twi_error = false;
      break;

    case NRF_DRV_TWI_EVT_ADDRESS_NACK:
#ifdef DEBUG_LOG
      NRF_LOG_INFO("TWI Address Not Found");
      NRF_LOG_FLUSH();
#endif

      // set transfer done to true
      m_xfer_done = true;
      // set twi error to true
      twi_error = true;
      break;

    case NRF_DRV_TWI_EVT_DATA_NACK:
#ifdef DEBUG_LOG
      NRF_LOG_INFO("TWI Transfer Failed");
      NRF_LOG_FLUSH();
#endif

      // set transfer done to true
      m_xfer_done = true;
      // set twi error to true
      twi_error = true;
      break;

    default:
      break;
  }
}


// TWI (I2C) initialization.
void twi_init (void)
{
  ret_code_t err_code;

  const nrf_drv_twi_config_t twi_lm75b_config = {
    .scl                = ARDUINO_SCL_PIN,
    .sda                = ARDUINO_SDA_PIN,
    .frequency          = NRF_DRV_TWI_FREQ_100K,
    .interrupt_priority = APP_IRQ_PRIORITY_HIGH,
    .clear_bus_init     = false
  };
  // this inits twi and sets the handler function
  err_code = nrf_drv_twi_init(&m_twi, &twi_lm75b_config, twi_handler, NULL);
  APP_ERROR_CHECK(err_code);

  nrf_drv_twi_enable(&m_twi);
}
