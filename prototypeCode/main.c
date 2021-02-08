/*
 * This program takes a temperature reading from the AD5933 and logs it via RTT.
 * An LED on the dev board blinks every time a temperature measurement is read.
 *
 * NOTICE: The current sdk_config.h file is set up so that the NRF_LOG buffer will block
 * 					the device from operation when its buffer is full. This means that not conneting
 *					to the device via RTT will cause the device to freeze. I will have to remind myself
 *					to disable this once I am done debugging
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
#include "app_error.h"
#include "nrf_drv_twi.h"

// logging includes
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

// AD5933 include
#include "AD5933.h"

// Needed to get the instance ID
#define TWI_INSTANCE_ID     0

// LED for USB status
#define LED_USB_RESUME      (BSP_BOARD_LED_0)

// if power detection is enabled
#ifndef USBD_POWER_DETECTION
#define USBD_POWER_DETECTION true
#endif

// USB defines
static void cdc_acm_user_ev_handler(app_usbd_class_inst_t const * p_inst, app_usbd_cdc_acm_user_event_t event);

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
static char m_rx_buffer[READ_SIZE];
static char m_tx_buffer[NRF_DRV_USBD_EPSIZE];

// Indicates if USB rx has occured
volatile bool rx_ready = false;

// function declarations
void twi_handler(nrf_drv_twi_evt_t const * p_event, void * p_context);
void twi_init (void);
static void usbd_user_ev_handler(app_usbd_event_type_t event);
void init_usb(void);

// get the TWI instance
const nrf_drv_twi_t m_twi = NRF_DRV_TWI_INSTANCE(TWI_INSTANCE_ID);

// Indicates if operation on TWI has ended
volatile bool m_xfer_done = false;

// Indicates TWI error
volatile bool twi_error = false;

/**
 * @brief Function for application main entry.
 */
int main(void)
{
	bool i2c_stats;						// stores if i2c success
	uint8_t AD5933_status;		// to store the status
	
  // create new sweep
	Sweep sweep;
	
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
	
	// reset the AD5933
	i2c_stats = AD5933_SetControl(NO_OPERATION, RANGE1, GAIN1, INTERN_CLOCK, 1);
	
	#ifdef DEBUG_LOG
	if (i2c_stats) {NRF_LOG_INFO("AD5933 Connected");}
	else {NRF_LOG_INFO("AD5933 Connection Failed");}
	NRF_LOG_FLUSH();
	#endif

	// Read the temp from the AD5933
	while (true)
	{
		// wait for USB to be ready
		while (app_usbd_event_queue_process());
		
		// if usb data is ready, send it back
		if (rx_ready)
		{
			uint8_t buff[1];
			app_usbd_cdc_acm_read(&m_app_cdc_acm, buff, 1);
			app_usbd_cdc_acm_write(&m_app_cdc_acm, buff, 1);
			rx_ready = false;
		}
		
		/* Sleep CPU only if there was no interrupt since last loop processing */
		__WFE();
	}
}

// USB event handler
static void cdc_acm_user_ev_handler(app_usbd_class_inst_t const * p_inst,
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

// USB user event handler
static void usbd_user_ev_handler(app_usbd_event_type_t event)
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

/**
 * @brief TWI events handler.
 */
void twi_handler(nrf_drv_twi_evt_t const * p_event, void * p_context)
{
	switch (p_event -> type)
	{
		case NRF_DRV_TWI_EVT_DONE:
      if (p_event -> xfer_desc.type == NRF_DRV_TWI_XFER_RX)
      {
        #ifdef DEBUG_LOG
        NRF_LOG_INFO("TWI Read Success");
        NRF_LOG_FLUSH();
        #endif
      }
      else if (p_event -> xfer_desc.type == NRF_DRV_TWI_XFER_TX)
      {
        #ifdef DEBUG_LOG
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

/**
 * @brief TWI (I2C) initialization.
 */
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
