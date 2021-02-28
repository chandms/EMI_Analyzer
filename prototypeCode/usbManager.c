#include<usbManager.h>

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

static uint8_t m_rx_buffer[READ_SIZE];
static uint8_t m_tx_buffer[NRF_DRV_USBD_EPSIZE];

// Indicates if USB rx has occured
static volatile bool rx_ready = false;
static volatile bool tx_ready = false;

// Writes numBytes from buff over USB
// Arguments:
//  * buff   - The buffer to write
//  numBytes - The number of bytes to write over usb
// Returns:
//  true if write success
//  false if write fail
bool usbManager_writeBytes(void * buff, uint32_t numBytes)
{
#ifdef DEBUG_USB
  NRF_LOG_INFO("Writing %d bytes over usb", numBytes);
  NRF_LOG_FLUSH();
#endif

  ret_code_t ret; // store the write status

  // check if numBytes exceeds the max that can be sent
  if (numBytes >= NRF_DRV_USBD_EPSIZE)
  {
#ifdef DEBUG_USB
    NRF_LOG_INFO("Write too large. Not writing.");
    NRF_LOG_FLUSH();
#endif
    return false;
  }
  
  // wait till tx_ready
  // while (!tx_ready);

  // reset tx_ready
  tx_ready = false;

  // write the bytes
	ret = app_usbd_cdc_acm_write(&m_app_cdc_acm, buff, numBytes);

  // check if fail
  if (ret != NRF_SUCCESS)
  {
#ifdef DEBUG_USB
    NRF_LOG_INFO("USB Write Fail");
    NRF_LOG_FLUSH();
#endif
    return false;
  }

#ifdef DEBUG_USB
    NRF_LOG_INFO("USB Write Success");
    NRF_LOG_FLUSH();
#endif
  // success
  return true;
}

// Reads numBytes over USB. Then it flushes the remaining bytes in the usb buffer.
// This function should only be used when the specific number of bytes being sent if known
// In order to get the first one byte command from the python script, use usbManager_getByte
// Arguments:
//  * buff   - The buffer to store the bytes
//  numBytes - The number of bytes to read from usb
// Returns:
//  true if read success
//  false if read fail
bool usbManager_readBytes(void * buff, uint32_t numBytes)
{
#ifdef DEBUG_USB
  NRF_LOG_INFO("Reading %d bytes over usb", numBytes);
  NRF_LOG_FLUSH();
#endif

  ret_code_t ret;          // to store read status

  // read numBytes from usb
	ret = app_usbd_cdc_acm_read(&m_app_cdc_acm, buff, numBytes);

  // check if fail
  if (ret != NRF_SUCCESS)
  {
#ifdef DEBUG_USB
    NRF_LOG_INFO("USB Read Fail");
    NRF_LOG_FLUSH();
#endif
    return false;
  }
	
	usbManager_flush();
  
  // reset rx_ready
  rx_ready = false;

#ifdef DEBUG_USB
    NRF_LOG_INFO("USB Read Success");
    NRF_LOG_FLUSH();
#endif

  return true;
}

// Gets the byte in m_rx_buffer, this is the first byte read from usb
// Arguments:
//  * buff   - The buffer to store the byte
// Returns:
//  true if read success
//  false if read fail
bool usbManager_getByte(uint8_t * buff)
{
  // check if rx_ready
  if (rx_ready)
  {
    *buff = m_rx_buffer[0];
		usbManager_flush();
		rx_ready = false;
    return true;
  }
  else
  {
    return false;
  }
}

// Flushes the usb buffer
void usbManager_flush(void)
{
	// clear the usb buffer
  uint8_t clear[1];
  while(app_usbd_cdc_acm_read(&m_app_cdc_acm, clear, 1) == NRF_SUCCESS);
	
	return;
}

// Returns the state of rx_ready. Also processes the queue
// Returns:
//  true if rx_ready is true
//  false if rx_ready is false
bool usbManager_readReady(void)
{
  // process the queue
  while(app_usbd_event_queue_process());
  
  // send back rx_ready
  return rx_ready;
}

// Inits usb using the function at the bottom of the file
// Returns:
//  true. If usb init fails, the device will simply restart
bool usbManager_init(void)
{
#ifdef DEBUG_USB
	NRF_LOG_INFO("USB Init");
	NRF_LOG_FLUSH();
#endif

  // init usb
  init_usb();

  // if it makes it here return true
  return true;
}


// USB event handler
void cdc_acm_user_ev_handler(app_usbd_class_inst_t const * p_inst,
    app_usbd_cdc_acm_user_event_t event)
{
  app_usbd_cdc_acm_t const * p_cdc_acm = app_usbd_cdc_acm_class_get(p_inst);

  switch (event)
  {
    case APP_USBD_CDC_ACM_USER_EVT_PORT_OPEN:
      {
        tx_ready = true;
        /*Setup first transfer*/
        ret_code_t ret = app_usbd_cdc_acm_read(&m_app_cdc_acm, m_rx_buffer, READ_SIZE);
        UNUSED_VARIABLE(ret);
        break;
      }
    case APP_USBD_CDC_ACM_USER_EVT_PORT_CLOSE:
		{
			tx_ready = false;
      rx_ready = false;
      break;
		}
    case APP_USBD_CDC_ACM_USER_EVT_TX_DONE:
		{
			tx_ready = true;
      break;
		}
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
      bsp_board_led_off(LED_USB_RESUME);
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
static void init_usb(void)
{
  ret_code_t ret;
  static const app_usbd_config_t usbd_config = {
    .ev_state_proc = usbd_user_ev_handler
  };

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
