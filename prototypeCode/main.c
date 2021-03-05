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
bool test(uint32_t * freq, uint16_t * real, uint16_t * imag, MetaData * metadata);

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
	Sweep oldSweep = {0};

  // set the sweep to default parameters
  set_default(&sweep);
	
	// variable to store the number of saved sweeps
	uint32_t numSweeps = 0;
	
	// variable to know what sweeps have been sent
	uint32_t pointer = 0;
	
	// load the config files from flash
	flashManager_checkConfig(&numSweeps, &oldSweep);

#ifdef DEBUG_LOG
  if (i2c_stats) {NRF_LOG_INFO("AD5933 Connected");}
  else {NRF_LOG_INFO("AD5933 Connection Failed");}
  NRF_LOG_FLUSH();
#endif
	
	uint8_t command[1]; // store the command from usb
	
  while (true)
  {
		// check if usb data is available, if it is get the first command
		if (usbManager_readReady() && usbManager_getByte(command))
		{
			NRF_LOG_INFO("%x", command[0]);
			NRF_LOG_FLUSH();
			
			// read the config file
			// send the number of saved sweeps
			if (command[0] == 1)
			{
				flashManager_checkConfig(&numSweeps, &oldSweep);
				usbManager_writeBytes(&numSweeps, sizeof(numSweeps));
				
				// also reset the pointer here
				pointer = numSweeps;
			}
			// execute a sweep and save to flash
			else if (command[0] == 2)
			{
				// allocate memory for sweep data
				uint32_t * freq = nrf_malloc(MAX_FREQ_SIZE);
				uint16_t * real = nrf_malloc(MAX_IMP_SIZE);
				uint16_t * imag = nrf_malloc(MAX_IMP_SIZE);
				
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
			// execute a sweep and immedietly send it over usb, do not save to flash
			else if (command[0] == 3)
			{
				// allocate memory for sweeps
				uint32_t * freq = nrf_malloc(MAX_FREQ_SIZE);
				uint16_t * real = nrf_malloc(MAX_IMP_SIZE);
				uint16_t * imag = nrf_malloc(MAX_IMP_SIZE);
				
				// execute the sweep
				if (AD5933_Sweep(&sweep, freq, real, imag))
				{
					NRF_LOG_INFO("Sending Sweep");
					NRF_LOG_FLUSH();
					if (usbManager_sendSweep(freq, real, imag, &sweep.metadata))
					{
						NRF_LOG_INFO("Sweep Send Success");
					}
					else
					{
						NRF_LOG_INFO("Sweep Send Fail");
					}
					NRF_LOG_FLUSH();
				}
				
				// free the memory
				nrf_free(freq);
				nrf_free(real);
				nrf_free(imag);
			}
			// send the pointer sweep over usb
			else if (command[0] == 4)
			{
				// allocate memory for sweeps
				uint32_t * freq = nrf_malloc(MAX_FREQ_SIZE);
				uint16_t * real = nrf_malloc(MAX_IMP_SIZE);
				uint16_t * imag = nrf_malloc(MAX_IMP_SIZE);
				
				// if pointer is at file 0, set it at the most recent sweep saved
				if (pointer == 0) pointer = numSweeps;
					
				// get the sweep data from flash
				if (flashManager_getSweep(freq, real, imag, &sweep.metadata, pointer))
				{
					if (usbManager_sendSweep(freq, real, imag, &sweep.metadata))
					{
						NRF_LOG_INFO("Sweep send from flash success");
						NRF_LOG_FLUSH();
					}
					else
					{
						NRF_LOG_INFO("Sweep send fail");
						NRF_LOG_FLUSH();
					}
				}
				
				// free memory
				nrf_free(freq);
				nrf_free(real);
				nrf_free(imag);
				
				// move pointer down
				pointer--;
			}
    }
    // Sleep CPU only if there was no interrupt since last loop processing
    __WFE();
	}
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
  sweep->delta 							= 1;
  sweep->steps 							= 99;
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
