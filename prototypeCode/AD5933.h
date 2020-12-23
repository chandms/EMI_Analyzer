/*
 *  AD5933.h
 *  
 *  Header file for the AD5933 library created for this project.
 *
 *  Author: Henry Silva
 *
 */

#ifndef INC_AD5933_H_
#define INC_AD5933_H_

#include <Wire.h>
#include <Arduino.h>

// Clock Frequency (for calculations)
// Internal clock is 16.776 MHz

#define CLK_FREQ 16776000

// Device address

#define AD5933_ADDR     0x0D

// Register definitions

#define CONTROL1_REG    0x80
#define CONTROL2_REG    0x81
#define START_FREQ_REG  0x82
#define DELTA_FREQ_REG  0x85
#define NUM_STEPS_REG   0x88
#define NUM_CYCLES_REG  0x8A
#define STATUS_REG      0x8F
#define TEMP_REG        0x92
#define IMPEDANCE_REG   0x94

// Settling Time Cycles Multipliers

#define TIMES1          0x00
#define TIMES2          0x01
#define TIMES4          0x03

// Operational Commands

#define NO_OPERATION    0x00
#define INIT_START_FREQ 0x01
#define START_SWEEP     0x02
#define INCREMENT_FREQ  0x03
#define REPEAT_FREQ     0x04
#define MEASURE_TEMP    0x09
#define POWER_DOWN      0x0A
#define STANDBY         0x0B

// Output Voltage Range

#define RANGE1          0x00
#define RANGE2          0x03
#define RANGE3          0x02
#define RANGE4          0x01

// PGA Gain

#define GAIN1           0x01
#define GAIN5           0x00

// Clock Source Setting

#define EXTERN_CLOCK    0x01
#define INTERN_CLOCK    0x00

// Status

#define STATUS_NONE     0x00
#define STATUS_TEMP     0x01
#define STATUS_DATA     0x02
#define STATUS_DONE     0x04

// Set pointer and block read/write command codes
#define SET_POINTER     0xB0
#define BLOCK_READ      0xA1
#define BLOCK_WRITE     0xA0

// AD5933 control functions
bool AD5933_SetStart(uint32_t start);
bool AD5933_SetDelta(uint32_t delta);
bool AD5933_SetSteps(uint16_t steps);
bool AD5933_SetCycles(uint16_t cycles, byte multiplier);
bool AD5933_SetControl(byte command, byte range, byte gain, byte clock);
bool AD5933_ReadStatus(byte * buff);
bool AD5933_ReadTemp(byte * buff);
bool AD5933_ReadData(byte * buff);

// Wire (I2C) helper functions
bool AD5933_SetPointer(byte reg);
bool AD5933_Write(byte reg, byte data);
bool AD5933_BlockWrite(byte * buff, uint8_t numBytes);
bool AD5933_BlockRead(byte * buff, uint8_t numBytes);
#endif /* INC_AD5933_H_ */
