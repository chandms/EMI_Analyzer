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

#include "nrf_drv_twi.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include <math.h>

// Clock Frequency (for calculations)
// Internal clock is 16.776 MHz

#define CLK_FREQ 16776000

// Device address (shifted right)

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

// get external variables from main
extern const nrf_drv_twi_t m_twi;
extern volatile bool m_xfer_done;

// AD5933 control functions
bool AD5933_SetStart(uint32_t start);
bool AD5933_SetDelta(uint32_t delta);
bool AD5933_SetSteps(uint16_t steps);
bool AD5933_SetCycles(uint16_t cycles, uint8_t multiplier);
bool AD5933_SetControl(uint8_t command, uint8_t range, uint8_t gain, uint8_t clock, uint8_t reset);
bool AD5933_ReadStatus(uint8_t * buff);
bool AD5933_ReadTemp(int * temp);
bool AD5933_ReadData(uint8_t * buff);

// Wire (I2C) helper functions
bool AD5933_SetPointer(uint8_t reg);
bool AD5933_Write(uint8_t reg, uint8_t data);
bool AD5933_ReadByte(uint8_t * buff);
bool AD5933_BlockWrite(uint8_t * buff, uint8_t numuint8_ts);
bool AD5933_BlockRead(uint8_t * buff, uint8_t numuint8_ts);
#endif /* INC_AD5933_H_ */
