/*
 *  ESP32_SD.cpp
 *
 *  Functions to control an SD card connected to the ESP32 via SPI.
 *
 *  Author: Henry Silva
 *
 */

#include "ESP32_SD.h"

bool ESP32_SD_readFile(fs::FS &fs, const char * path, unsigned char * buff, uint8_t numBytes)
{
  // open file. If fail, return false
  File file = fs.open(path, FILE_READ);
  if (!file) return false;

  // read from the file and check for error
  if (file.read(buff, numBytes) != numBytes) 
  {
    file.close();
    return false;
  }

  // close the file
  file.close();

  // read success
  return true;
}

bool ESP32_SD_writeFile(fs::FS &fs, const char * path, const char * message)
{
  // open file and check if worked
  File file = fs.open(path, FILE_WRITE);
  if(!file) return false;

  // write to the file and check if fail
  if(!file.print(message))
  {
    file.close();
    return false;
  }

  // close the file
  file.close();

  return true;
}

bool ESP32_SD_appendFile(fs::FS &fs, const char * path, const char * message)
{
  // open file and check if worked
  File file = fs.open(path, FILE_APPEND);
  if(!file) return false;

  // write to the file and check if fail
  if(!file.print(message))
  {
    file.close();
    return false;
  }

  // close the file
  file.close();

  return true;
}

bool ESP32_SD_mountSD()
{
  // try to mount SD card. If fail, return false
  if (!SD.begin()) return false;

  // determine card type. If no type. no SD card is attatched
  uint8_t cardType = SD.cardType();
  if (cardType == CARD_NONE) return false;

  // if everything worked return true
  return true;
}
