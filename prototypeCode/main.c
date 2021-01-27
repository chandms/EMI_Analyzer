/*
 * This program takes a temperature reading from the AD5933 and logs it via RTT.
 * An LED on the dev board blinks every time a temperature measurement is read.
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

/**
 * @brief Function for application main entry.
 */
int main(void)
{
	// init LEDS
	bsp_board_init(BSP_INIT_LEDS);
	
	// init Log
	APP_ERROR_CHECK(NRF_LOG_INIT(NULL));
	NRF_LOG_DEFAULT_BACKENDS_INIT();
	NRF_LOG_INFO("Wireless Sensor Started");
	NRF_LOG_FLUSH();

	// init twi
	twi_init();
	
	// stores if i2c success
	bool i2c_stats;
	
	// to store the status
	uint8_t AD5933_status;
	
	// to store the temperature
	int temp;

	// Read the temp from the AD5933
	while (true)
	{
		// reset the AD5933
		i2c_stats = AD5933_SetControl(NO_OPERATION, RANGE1, GAIN1, INTERN_CLOCK, 1);
		if (i2c_stats) {NRF_LOG_INFO("AD5933 Reset")}
		else {NRF_LOG_INFO("AD5933 Reset Failed")}
		NRF_LOG_FLUSH();
		
		// read the status of the AD5933
		i2c_stats = AD5933_ReadStatus(&AD5933_status);
		
		// set the AD5933 to read the temp
		i2c_stats = AD5933_SetControl(MEASURE_TEMP, RANGE1, GAIN1, INTERN_CLOCK, 0);
		if (i2c_stats) {NRF_LOG_INFO("Measuring Temperature")}
		else {NRF_LOG_INFO("Temp command failed")}
		NRF_LOG_FLUSH();
		
		// read status until temp has been read
		do{
			i2c_stats = AD5933_ReadStatus(&AD5933_status);
			nrf_delay_ms(100);
		} while ((AD5933_status & 0x01) != STATUS_TEMP);
		NRF_LOG_INFO("Temp Measurement Ready");
		NRF_LOG_FLUSH();
		
		// read the temp data
		AD5933_ReadTemp(&temp);
		NRF_LOG_INFO("AD5933 Temperature: %d degrees C\n", temp);
		NRF_LOG_FLUSH();
		
		// blink LED then wait
		bsp_board_led_invert(0);
		nrf_delay_ms(100);
		bsp_board_led_invert(0);
		
		nrf_delay_ms(1000);
	}
}

/**
 * @brief TWI events handler.
 */
void twi_handler(nrf_drv_twi_evt_t const * p_event, void * p_context)
{
    switch (p_event->type)
    {
        case NRF_DRV_TWI_EVT_DONE:
            if (p_event->xfer_desc.type == NRF_DRV_TWI_XFER_RX)
            {
							// put data handler function here
              // data_handler(m_sample);
            }
						// sets transfer done to true
            m_xfer_done = true;
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
