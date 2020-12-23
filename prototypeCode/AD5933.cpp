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
  byte buff[3];

  // calculate start frequency code
  start = ((float) start / (CLK_FREQ / 4)) * pow(2, 27);

  // cut code into 3 bytes (most significant byte not needed)
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

bool AD5933_SetCycles(uint16_t cycles, byte multiplier)
{
  return true;
}

bool AD5933_SetControl(byte command, byte range, byte gain, byte clock)
{
  return true;
}

bool AD5933_ReadStatus(byte * buff)
{
  return true;
}

bool AD5933_ReadTemp(byte * buff)
{
  return true;
}

bool AD5933_ReadData(byte * buff)
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
bool AD5933_SetPointer(byte reg)
{
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
}

// Writes one byte to the specified register
// Arguments:
//  reg  - The register to write to
//  data - The byte to write
// Return value:
//  false if I2C error
//  true if no error
bool AD5933_Write(byte reg, byte data)
{
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
}

// Write numBytes to the location of the internal pointer (set the pointer with AD5933_SetPointer)
// Arguments:
//  buff     - Pointer to byte array to write from
//  numBytes - Number of bytes to write
// Return value:
//  false if I2C error
//  true if no error
bool AD5933_BlockWrite(byte * buff, uint8_t numBytes)
{
  // begin I2C transmission
  Wire.beginTransmission(AD5933_ADDR);

  // write block write command code
  Wire.write(BLOCK_WRITE);

  // write number of bytes being written
  Wire.write(numBytes);

  // write numBytes from buff
  Wire.write(buff, numBytes);

  // end transmission. If fail then return false
  if (Wire.endTransmission()) return false;

  // transmission success
  return true;
}

// Read numBytes from the location of the internal pointer (set the pointer with AD6933_setPointer)
// Arguments:
//  buff     - Pointer to the byte array to input the read data
//  numBytes - The number of bytes to read
// Return value:
//  false if I2C error
//  true if no error
bool AD5933_BlockRead(byte * buff, uint8_t numBytes)
{
  // begin I2C transmission
  Wire.beginTransmission(AD5933_ADDR);

  // write block read command code
  Wire.write(BLOCK_READ);

  // write number of bytes to read
  Wire.write(numBytes);

  // end transmission. If fail return false
  if (Wire.endTransmission()) return false;

  // request numBytes from AD5933. Check if the number of bytes recieved is correct
  if (Wire.requestFrom(AD5933_ADDR, numBytes) != (byte) numBytes) return false;

  // keep track of the number of bytes read
  uint8_t numRead = 0;

  // read bytes until none are left
  while (Wire.available())
  {
    // store byte in buff
    buff[numRead] = Wire.read();
    numRead++;
  }

  // check if numRead is equal to numBytes
  if (numRead != numBytes) return false;
  
  // read success
  return true;
}
