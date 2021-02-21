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
#include "nrf_drv_usbd.h"
#include "nrf_drv_clock.h"
#include "nrf_gpio.h"
#include "nrf_drv_power.h"
#include "app_error.h"
#include "app_util.h"
#include "app_usbd_core.h"
#include "app_usbd.h"
#include "app_usbd_string_desc.h"
#include "app_usbd_cdc_acm.h"
#include "app_usbd_serial_num.h"
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

// --- User Defines ---

bool recieveSweep(Sweep * sweep);
void set_default(Sweep * sweep);
bool sendSweep(uint32_t sweepNum);

// --- USB Defines ---

void cdc_acm_user_ev_handler(app_usbd_class_inst_t const * p_inst, app_usbd_cdc_acm_user_event_t event);
void usbd_user_ev_handler(app_usbd_event_type_t event);
void init_usb(void);

// if power detection is enabled
#ifndef USBD_POWER_DETECTION
#define USBD_POWER_DETECTION true
#endif

// LED for USB status
#define LED_USB_RESUME      (BSP_BOARD_LED_0)

#define CDC_ACM_COMM_INTERFACE  0
#define CDC_ACM_COMM_EPIN       NRF_DRV_USBD_EPIN2

#define CDC_ACM_DATA_INTERFACE  1
#define CDC_ACM_DATA_EPIN       NRF_DRV_USBD_EPIN1
#define CDC_ACM_DATA_EPOUT      NRF_DRV_USBD_EPOUT1

// USB acm class instance
APP_USBD_CDC_ACM_GLOBAL_DEF(m_app_cdc_acm,
    cdc_acm_user_ev_handler,
    CDC_ACM_COMM_INTERFACE,
    CDC_ACM_DATA_INTERFACE,
    CDC_ACM_COMM_EPIN,
    CDC_ACM_DATA_EPIN,
    CDC_ACM_DATA_EPOUT,
    APP_USBD_CDC_COMM_PROTOCOL_AT_V250
    );

#define READ_SIZE 1
uint8_t m_rx_buffer[READ_SIZE];
uint8_t m_tx_buffer[NRF_DRV_USBD_EPSIZE];

// Indicates if USB rx has occured
volatile bool rx_ready = false;
volatile bool tx_ready = false;

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
  init_usb();
	
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
	
  while (true)
  {
    // wait for USB to be ready
    while (app_usbd_event_queue_process());

    // if there is data over usb, read one byte
    if (rx_ready)
    {
      do
      {
        // using usb for testing

        // starting with '1' on the keyboard is equal to 49
        if (m_rx_buffer[0] == 49)
        {
          flashManager_checkConfig(&numSweeps, &sweep);
        }
        if (m_rx_buffer[0] == 50)
        {
					// allocate memory for sweep data
					freq = nrf_malloc(sizeof(uint32_t) * (sweep.metadata.numPoints));
					real = nrf_malloc(sizeof(uint16_t) * (sweep.metadata.numPoints));
					imag = nrf_malloc(sizeof(uint16_t) * (sweep.metadata.numPoints));
					
					if (AD5933_Sweep(&sweep, freq, real, imag))
					{
						if (flashManager_saveSweep(freq, real, imag, &sweep.metadata, numSweeps + 1))
						{
							numSweeps += 1;
							flashManager_updateNumSweeps(&numSweeps);
							NRF_LOG_INFO("Sweep %d saved, freq: %d real: %d imag: %d", numSweeps, freq[3], real[3], imag[3]);
							NRF_LOG_FLUSH();
						}
					}
					// free the memory
					nrf_free(freq);
					nrf_free(real);
					nrf_free(imag);
        }
        if (m_rx_buffer[0] == 51)
        {
					// allocate memory for sweep data
					freq = nrf_malloc(sizeof(uint32_t) * (sweep.metadata.numPoints));
					imag = nrf_malloc(sizeof(uint16_t) * (sweep.metadata.numPoints));
					real = nrf_malloc(sizeof(uint16_t) * (sweep.metadata.numPoints));

          if (flashManager_getSweep(freq, real, imag, &sweep.metadata, numSweeps))
					{
						NRF_LOG_INFO("Sweep %d found, freq: %d real: %d imag: %d", numSweeps, freq[3], real[3], imag[3]);
						NRF_LOG_FLUSH();
					}
					// free the memory
					nrf_free(freq);
					nrf_free(real);
					nrf_free(imag);
        }
				if (m_rx_buffer[0] == 3)
				{
					// send back 3
					uint8_t buff[1] = {3};
					app_usbd_cdc_acm_write(&m_app_cdc_acm, buff, 1);
					
					// send the most recent sweep
					if (sendSweep(numSweeps))
					{
						NRF_LOG_INFO("Sweep send success");
					}
					else
					{
						NRF_LOG_INFO("Sweep send fail");
					}
					NRF_LOG_FLUSH();
				}

        ret = app_usbd_cdc_acm_read(&m_app_cdc_acm, m_rx_buffer, 1);
      } while(ret == NRF_SUCCESS);
      // reset rx_ready
      bsp_board_led_invert(1);
      rx_ready = false;
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
			app_usbd_cdc_acm_write(&m_app_cdc_acm, buff, 8);
		}
		ret = true;
	}
	
	// send a blank sweep to indicate sweep done
	uint8_t blank[8] = {0};
	app_usbd_cdc_acm_write(&m_app_cdc_acm, blank, 8);
	
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

  app_usbd_cdc_acm_read(&m_app_cdc_acm, params, 16);

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

// --- USB Functions ---

// USB event handler
void cdc_acm_user_ev_handler(app_usbd_class_inst_t const * p_inst,
    app_usbd_cdc_acm_user_event_t event)
{
  app_usbd_cdc_acm_t const * p_cdc_acm = app_usbd_cdc_acm_class_get(p_inst);

  switch (event)
  {
    case APP_USBD_CDC_ACM_USER_EVT_PORT_OPEN:
      {
        /*Setup first transfer*/
        ret_code_t ret = app_usbd_cdc_acm_read(&m_app_cdc_acm, m_rx_buffer, READ_SIZE);
        UNUSED_VARIABLE(ret);
        break;
      }
    case APP_USBD_CDC_ACM_USER_EVT_PORT_CLOSE:
      break;
    case APP_USBD_CDC_ACM_USER_EVT_TX_DONE:
      break;
    case APP_USBD_CDC_ACM_USER_EVT_RX_DONE:
      {
        // notify something that data is ready to be read
        rx_ready = true;
        break;
      }
    default:
      break;
  }
}

// USB driver event handler
void usbd_user_ev_handler(app_usbd_event_type_t event)
{
  switch (event)
  {
    case APP_USBD_EVT_DRV_SUSPEND:
      bsp_board_led_off(LED_USB_RESUME);
      break;
    case APP_USBD_EVT_DRV_RESUME:
      bsp_board_led_on(LED_USB_RESUME);
      break;
    case APP_USBD_EVT_STARTED:
      break;
    case APP_USBD_EVT_STOPPED:
      app_usbd_disable();
      bsp_board_leds_off();
      break;
    case APP_USBD_EVT_POWER_DETECTED:
      if (!nrf_drv_usbd_is_enabled())
      {
        app_usbd_enable();
      }
      break;
    case APP_USBD_EVT_POWER_REMOVED:
      app_usbd_stop();
      break;
    case APP_USBD_EVT_POWER_READY:
      app_usbd_start();
      break;
    default:
      break;
  }
}

// USB initialization
void init_usb(void)
{
  ret_code_t ret;
  static const app_usbd_config_t usbd_config = {
    .ev_state_proc = usbd_user_ev_handler
  };

  ret = NRF_LOG_INIT(NULL);
  APP_ERROR_CHECK(ret);

  ret = nrf_drv_clock_init();
  APP_ERROR_CHECK(ret);

  nrf_drv_clock_lfclk_request(NULL);

  while(!nrf_drv_clock_lfclk_is_running())
  {
    /* Just waiting */
  }

  ret = app_timer_init();
  APP_ERROR_CHECK(ret);

  app_usbd_serial_num_generate();

  ret = app_usbd_init(&usbd_config);
  APP_ERROR_CHECK(ret);

  app_usbd_class_inst_t const * class_cdc_acm = app_usbd_cdc_acm_class_inst_get(&m_app_cdc_acm);
  ret = app_usbd_class_append(class_cdc_acm);
  APP_ERROR_CHECK(ret);

  if (USBD_POWER_DETECTION)
  {
    ret = app_usbd_power_events_enable();
    APP_ERROR_CHECK(ret);
  }
  else
  {
    app_usbd_enable();
    app_usbd_start();
  }
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
