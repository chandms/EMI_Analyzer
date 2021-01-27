/*
 *  ESP32_SD.h
 *
 *  Header file to interface with an SD card connected to the ESP32 via SPI using espressif's SD card library
 *
 *  Author: Henry Silva
 *
 */

#ifndef INC_ESP32_SD_H_
#define INC_ESP32_SD_H_

#include <Arduino.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"

// functions
bool ESP32_SD_readFile(fs::FS &fs, const char * path);
bool ESP32_SD_writeFile(fs::FS &fs, const char * path, const char * message);
bool ESP32_SD_appendFile(fs::FS &fs, const char * path, const char * message);
bool ESP32_SD_mountSD();
#endif
