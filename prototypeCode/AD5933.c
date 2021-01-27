/*
 *  AD5933.cpp
 *
 *  Functions to control the AD5933. All conversions are done within the functions.
 *
 *  Author: Henry Silva
 *
 */

#include "AD5933.h"

// sets the start frequency of the frequency sweep
// Arguments: start - The start frequency in Hz
bool AD5933_SetStart(uint32_t start)
{
  // check if start is within output frequency range
  if (start < 1000 || start > 100000) return false;

  // create buffer to store data to send
  uint8_t buff[3];

  // calculate start frequency code
  start = ((float) start / (CLK_FREQ / 4)) * pow(2, 27);

  // cut code into 3 uint8_ts (most significant uint8_t not needed)
  buff[0] = (start >> 16) & 0xFF;
  buff[1] = (start >> 8) & 0xFF;
  buff[2] = start & 0xFF;

  return true;
}

bool AD5933_SetDelta(uint32_t delta)
{
  return true;
}

bool AD5933_SetSteps(uint16_t steps)
{
  return true;
}

bool AD5933_SetCycles(uint16_t cycles, uint8_t multiplier)
{
  return true;
}

// sets the control registers of the AD5933
// Arguments: 
//  command - the control function of the AD5933
//	range   - output excitation voltage range
//	gain		- PGA gain of the response signal
//	clock		- selects the internal or external clock
//	reset   - reset command (1 for reset, 0 for no reset)
// Return value:
//  false if I2C error
//  true if no error
bool AD5933_SetControl(uint8_t command, uint8_t range, uint8_t gain, uint8_t clock, uint8_t reset)
{
	// create buffer to store commands
	uint8_t buff[2];

  // combine the commands
  buff[0] = (command << 4) | ((range << 2) | gain);
  buff[1] = (reset << 4) | (clock << 3);
	
	#ifdef DEBUG
	NRF_LOG_INFO("Control Byte 0: %x", buff[0]);
	NRF_LOG_INFO("Control Byte 1: %x", buff[1]);
	NRF_LOG_FLUSH();
	#endif
	
  // write to each register one at a time (the datasheet specifies that a block write command cannot be used)
  // check for error
  if (!AD5933_Write(CONTROL1_REG, buff[0])) return false;
  return AD5933_Write(CONTROL2_REG, buff[1]);
}

// reads the status of the AD5933 and puts the data into buff
// Arguments: 
//  buff - Pointer to the buffer that stores the data
// Return value:
//  false if I2C error
//  true if no error
bool AD5933_ReadStatus(uint8_t * buff)
{
  // set the pointer and check for failure
  if (!AD5933_SetPointer(STATUS_REG)) return false;
	
	// read the status byte and return result
  return AD5933_ReadByte(buff);
}

// reads the temperature register of the AD5933 and converts it to celcius. Must first send a Measure Temperature command via AD5933_SetControl
// Arguments: 
//  * temp - the pointer to the integer that will store the temperature in Celcius
// Return value:
//  false if I2C error
//  true if no error
bool AD5933_ReadTemp(int * temp)
{
  // create buffer to store temperature data
  uint8_t buff[2];

  // set the pointer and check for failure
  if (!AD5933_SetPointer(TEMP_REG)) return false;
	
	// block 2 bytes from the temperature register
  if (!AD5933_BlockRead(buff, 2)) return false;

  // check if temp is negative
	if ((buff[0] & 0x20))
	{
		*temp = -1;
	}
	else
	{
		// put the data into a 16 bit unsigned int
		uint16_t num = buff[0];
		num = (num << 8) | buff[1];
		
		// calculate
		*temp = num / 32;
	}
  return true;
}

bool AD5933_ReadData(uint8_t * buff)
{
  return true;
}

// Wire helper functions

// Sets the internal pointer of the AD5933. Usually do this before block read or write
// Arguments: 
//  reg - The register to point to
// Return value:
//  false if I2C error
//  true if no error
bool AD5933_SetPointer(uint8_t reg)
{
	// stores error code
	ret_code_t err_code;
	
	// set data buffer
	uint8_t buff[2] = {SET_POINTER, reg};
	
	// send the data
	m_xfer_done = false;
	err_code = nrf_drv_twi_tx(&m_twi, AD5933_ADDR, buff, sizeof(buff), false);
	
	// check for error
	APP_ERROR_CHECK(err_code);
	
	// wait for transfer to be done
	while (m_xfer_done == false);
	
	// check if success
	if (err_code != NRF_SUCCESS) return false;
	
	// success
	return true;
	
	/* ORIGINAL CODE FOR ESP32
  // begin I2C transmission
  Wire.beginTransmission(AD5933_ADDR);

  // write pointer command
  Wire.write(SET_POINTER);

  // write register to point to
  Wire.write(reg);

  // end transmission. If transmission failed then return false
  if (Wire.endTransmission()) return false;

  // transmission success
  return true;
	*/
}

// Writes one uint8_t to the specified register
// Arguments:
//  reg  - The register to write to
//  data - The uint8_t to write
// Return value:
//  false if I2C error
//  true if no error
bool AD5933_Write(uint8_t reg, uint8_t data)
{
	// stores error code
	ret_code_t err_code;
	
	// set data buffer
	uint8_t buff[2] = {reg, data};
	
	// send the data
	m_xfer_done = false;
	err_code = nrf_drv_twi_tx(&m_twi, AD5933_ADDR, buff, sizeof(buff), false);
	
	// check for error
	APP_ERROR_CHECK(err_code);
	
	// wait for transfer to be done
	while (m_xfer_done == false);
	
	// check if success
	if (err_code != NRF_SUCCESS) return false;
	
	// success
	return true;
	
	/* ORIGINAL ESP32 CODE
  // begin I2C transmission
  Wire.beginTransmission(AD5933_ADDR);

  // write address to send data
  Wire.write(reg);

  // write data to register
  Wire.write(data);

  // end transmission. If fail then return false
  if (Wire.endTransmission()) return false;

  // transmission success
  return true;
	*/
}

// Write numbytes (max 30) to the location of the internal pointer (set the pointer with AD5933_SetPointer)
// Arguments:
//  buff     - Pointer to uint8_t array to write from
//  numbytes - Number of bytes to write
// Return value:
//  false if I2C error
//  true if no error
bool AD5933_BlockWrite(uint8_t * buff, uint8_t numbytes)
{
	// stores error code
	ret_code_t err_code;
	
	// check if numbytes greater than 32
	if (numbytes > 30)
	{
		return false;
	}
	
	// create data buffer. 
	uint8_t data[32];
	
	// set block write and number of bytes
	data[0] = BLOCK_WRITE;
	data[1] = numbytes;
	
	// set the rest of the data
	for (int i = 0; i < numbytes; i++)
	{
		data[i + 2] = buff[i];
	}
	
	// send the data
	m_xfer_done = false;
	err_code = nrf_drv_twi_tx(&m_twi, AD5933_ADDR, data, sizeof(data), false);
	
	// check for error
	APP_ERROR_CHECK(err_code);
	
	// wait for transfer to be done
	while (m_xfer_done == false);
	
	// check if success
	if (err_code != NRF_SUCCESS) return false;
	
	// success
	return true;	
	
	/*
  // begin I2C transmission
  Wire.beginTransmission(AD5933_ADDR);

  // write block write command code
  Wire.write(BLOCK_WRITE);

  // write number of uint8_ts being written
  Wire.write(numbytes);

  // write numbytes from buff
  Wire.write(buff, numbytes);

  // end transmission. If fail then return false
  if (Wire.endTransmission()) return false;

  // transmission success
  return true;
	*/
}

// Read one byte from the location of the internal pointer (set the pointer with AD6933_setPointer)
// Arguments:
//  buff     - Pointer to the uint8_t to input the read byte
// Return value:
//  false if I2C error
//  true if no error
bool AD5933_ReadByte(uint8_t * buff)
{
	// stores error code
	ret_code_t err_code;

	// read byte from AD5933
	m_xfer_done = false;
	err_code = nrf_drv_twi_rx(&m_twi, AD5933_ADDR, buff, 1);
	
	// check for error
	APP_ERROR_CHECK(err_code);
	
	// wait for transfer to be done
	while (m_xfer_done == false);
	
	// check if success
	if (err_code != NRF_SUCCESS) return false;
	
	// success
	return true;
}

// Read numbytes from the location of the internal pointer (set the pointer with AD6933_setPointer)
// Arguments:
//  buff     - Pointer to the uint8_t array to input the read data
//  numbytes - The number of uint8_ts to read
// Return value:
//  false if I2C error
//  true if no error
bool AD5933_BlockRead(uint8_t * buff, uint8_t numbytes)
{
	// stores error code
	ret_code_t err_code;
	
	// set data buffer
	uint8_t data[2] = {BLOCK_READ, numbytes};
	
	// send the data to initiate block read
	m_xfer_done = false;
	err_code = nrf_drv_twi_tx(&m_twi, AD5933_ADDR, data, sizeof(data), false);
	
	// check for error
	APP_ERROR_CHECK(err_code);
	
	// wait for transfer to be done
	while (m_xfer_done == false);
	
	// check if success
	if (err_code != NRF_SUCCESS) return false;
	
	// now read numbytes from the AD5933
	m_xfer_done = false;
	err_code = nrf_drv_twi_rx(&m_twi, AD5933_ADDR, buff, numbytes);
	
	// check for error
	APP_ERROR_CHECK(err_code);
	
	// wait for transfer to be done
	while (m_xfer_done == false);
	
	// check if success
	if (err_code != NRF_SUCCESS) return false;
	
	// success
	return true;
	
	/* ORIGINAL ESP32 CODE
  // begin I2C transmission
  Wire.beginTransmission(AD5933_ADDR);

  // write block read command code
  Wire.write(BLOCK_READ);

  // write number of uint8_ts to read
  Wire.write(numbytes);

  // end transmission. If fail return false
  if (Wire.endTransmission()) return false;

  // request numbytes from AD5933. Check if the number of uint8_ts recieved is correct
  if (Wire.requestFrom(AD5933_ADDR, numbytes) != (uint8_t) numbytes) return false;

  // keep track of the number of uint8_ts read
  uint8_t numRead = 0;

  // read uint8_ts until none are left
  while (Wire.available())
  {
    // store uint8_t in buff
    buff[numRead] = Wire.read();
    numRead++;
  }

  // check if numRead is equal to numbytes
  if (numRead != numbytes) return false;
  
  // read success
  return true;
	*/
}
