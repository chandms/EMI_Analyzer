/*
 * This program takes a temperature reading from the AD5933 and logs it via RTT.
 * An LED on the dev board blinks every time a temperature measurement is read.
 *
 * WARNING: The current sdk_config.h file is set up so that the NRF_LOG buffer will block
 * 					the device from operation when its buffer is full. This means that not conneting
 *					to the device via RTT will cause the device to freeze. I will have to remind myself
 *					to disable this once I am done debugging
 */

// standard includes
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
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

// function declarations
void twi_handler(nrf_drv_twi_evt_t const * p_event, void * p_context);
void twi_init (void);

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
	
  // create new sweep and set parameters
	Sweep sweep;

	sweep.start = 1000;
	sweep.delta = 100;
	sweep.steps = 5;
	sweep.cycles = 15;
	
	sweep.cyclesMultiplier = NO_MULT;
	sweep.clockFrequency = CLK_FREQ;
	sweep.clockSource = INTERN_CLOCK;
	sweep.range = RANGE1;
	sweep.gain = GAIN1;
	sweep.gainFactor[0] = 0;
	sweep.gainFactor[1] = 0;
	
	// init LEDS
	bsp_board_init(BSP_INIT_LEDS);
	
	// init Log
	APP_ERROR_CHECK(NRF_LOG_INIT(NULL));
	NRF_LOG_DEFAULT_BACKENDS_INIT();
	NRF_LOG_INFO("Wireless Sensor Started");
	NRF_LOG_FLUSH();

	// init twi
	twi_init();

	// Read the temp from the AD5933
	while (true)
	{
		NRF_LOG_INFO("-----------------------------------");
		NRF_LOG_FLUSH();
		
		// reset the AD5933
		i2c_stats = AD5933_SetControl(NO_OPERATION, RANGE1, GAIN1, INTERN_CLOCK, 1);
		if (i2c_stats) {NRF_LOG_INFO("AD5933 Connected");}
		else {NRF_LOG_INFO("AD5933 Connection Failed");}
		NRF_LOG_FLUSH();
		
		// if the device is successfully connected continue, if not long blink LED 2
		if (i2c_stats)
		{
			// calculate gain factor
			AD5933_CalculateGainFactor(&sweep, 1000);
			
			NRF_LOG_INFO("Starting Sweep");
			NRF_LOG_FLUSH();
			
			// start a frequency sweep
			AD5933_StartSweep(&sweep);

			// blink LED 1 to indicate success
			bsp_board_led_invert(0);
			nrf_delay_ms(100);
			bsp_board_led_invert(0);
		}
		else
		{
			// long blink LED 2 if twi failed
			bsp_board_led_invert(1);
			nrf_delay_ms(500);
			bsp_board_led_invert(1);
		}
		
		// wait a second
		nrf_delay_ms(5000);
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
 * @brief UART (I2C) initialization.
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
