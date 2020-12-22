#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include <Wire.h>

// TODO
// replace read file to send bytes to char array
// create function that reads a config file and sets parameters
// create function that generates a log file and writes to it

// reads from a file and outputs to serial monitor. Returns true if success
bool readFile(fs::FS &fs, const char * path)
{
    // open file. If fail, return false
    File file = fs.open(path, FILE_READ);
    if (!file) return false;

    // read from the file till the end
    while (file.available()) Serial.write(file.read());
    
    // close the file
    file.close();

    return true;
}

// writes to a file
bool writeFile(fs::FS &fs, const char * path, const char * message)
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

// append to a file
bool appendFile(fs::FS &fs, const char * path, const char * message)
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

// checks if SD card is mounted. Returns true if SD card is found
bool mountSD()
{
  // try to mount SD card. If fail, return false
  if (!SD.begin()) return false;

  // determine card type. If no type. no SD card is attatched
  uint8_t cardType = SD.cardType();
  if (cardType == CARD_NONE) return false;

  // if everything worked return true
  return true;
}

void setup(){
    // begin Serial monitor on 115200 baud
    Serial.begin(115200);

    // mount the SD card
    if (!mountSD()) Serial.println("No SD Card Detected");

    // create a sample csv with headers
    writeFile(SD, "/test.csv", "Frequency,Impedance,Phase,Real,Imaginary,Magnitude\n");

    // create a data sample from a previous concrete test
    char buff[100];
    sprintf(buff, "%d,%.10f,%.10f,%d,%d,%.10f\n", 1000, 1252.68722415183, -26.4870439248944, -16103, -1843, 16208.1232102918);

    // record start time of SD append
    uint32_t start = millis();
    // append to the SD card
    appendFile(SD, "/test.csv", buff);

    // record end time and print
    uint32_t tim = millis() - start;
    Serial.printf("SD append took %u ms to write %d bytes.\n", tim, strlen(buff));

    // read it
    readFile(SD, "/test.csv");

    // test complete
    Serial.println("Test Complete");
}

void loop(){

}

/* below are examples of commands

The ESP32 uses the Wire library to send and recieve data via I2C
Here is an explanation of all the Wire commands used in the examples
  Wire.beginTransmission(ADDRESS);     // Starts an I2C transmission to the device at address ADDRESS
  Wire.endTransmission();              // Completes the current I2C transmission and sends it
  Wire.write(BYTE);                    // Adds one BYTE to the current I2C transmission
  Wire.requestFrom(ADDRESS,NUM_BYTES); // Reads NUM_BYTES from device at ADDRESS

To set the start frequency of the frequency sweep to 1kHz (using the 16.776MHz internal clock)
AD5933 Command: 
  block write 0x007D02 to register 0x82
ESP32 Code:
  // first set the AD5933's pointer to register 0x82
  Wire.beginTransmission(0x0D);
  Wire.write(0xB0); // write pointer command code
  Wire.write(0x82); // write the register to point to
  Wire.endTransmission();
  // then block write 3 bytes to the AD5933
  Wire.beginTransmission(0x0D);
  Wire.write(0xA0); // write block write command code
  Wire.write(0x03); // write number of bytes being written
  Wire.write(0x00); // write first byte
  Wire.write(0x7D); // write second byte
  Wire.write(0x02); // write third byte
  Wire.endTransmission();

To set the frequncy increment to 100Hz (using the 16.776MHz internal clock)
AD5933 Command: 
  block write 0x000C80 to register 0x85
ESP32 Code:
  // first set the AD5933's pointer to register 0x85
  Wire.beginTransmission(0x0D);
  Wire.write(0xB0); // write pointer command code
  Wire.write(0x85); // write the register to point to
  Wire.endTransmission();
  // then block write 3 bytes to the AD5933
  Wire.beginTransmission(0x0D);
  Wire.write(0xA0); // write block write command code
  Wire.write(0x03); // write number of bytes being written
  Wire.write(0x00); // write first byte
  Wire.write(0x0C); // write second byte
  Wire.write(0x80); // write third byte
  Wire.endTransmission();

To set the number of increments to 490
AD5933 Command: 
  block write 0x01EA to register 0x88
ESP32 Code:
  // first set the AD5933's pointer to register 0x88
  Wire.beginTransmission(0x0D);
  Wire.write(0xB0); // write pointer command code
  Wire.write(0x88); // write the register to point to
  Wire.endTransmission();
  // then block write 2 bytes to the AD5933
  Wire.beginTransmission(0x0D);
  Wire.write(0xA0); // write block write command code
  Wire.write(0x02); // write number of bytes being written
  Wire.write(0x01); // write first byte
  Wire.write(0xEA); // write second byte
  Wire.endTransmission();

To set the number of settling time cycles to 15
AD5933 Command: 
  block write 0x000F to register 0x8A
ESP32 Code:
  // first set the AD5933's pointer to register 0x88
  Wire.beginTransmission(0x0D);
  Wire.write(0xB0); // write pointer command code
  Wire.write(0x8A); // write the register to point to
  Wire.endTransmission();
  // then block write 2 bytes to the AD5933
  Wire.beginTransmission(0x0D);
  Wire.write(0xA0); // write block write command code
  Wire.write(0x02); // write number of bytes being written
  Wire.write(0x00); // write first byte
  Wire.write(0x0F); // write second byte
  Wire.endTransmission();

To set the mode of the AD5933
Modes with hex command:
  Power Down  0xA1
  Standby     0xB1
  Initialize  0x11
  Start Sweep 0x21
  Increment   0x31
  Repeat      0x41
  Meas. Temp. 0x91
AD5933 Command: 
  write mode to register 0x80
ESP32 Code:
  Wire.beginTransmission(0x0D);
  Wire.write(0x80); // register address
  Wire.write(MODE); // write mode
  Wire.endTransmission();

To read the status register of the AD5933
Status Codes:
  0x01 Valid temperature measurement
  0x02 Valid real/imaginary data
  0x04 Frequency sweep complete
AD5933 Command:
  block read 1 byte from register 0x8F
ESP32 Command:
  // first set the AD5933's pointer to register 0x8F
  Wire.beginTransmission(0x0D);
  Wire.write(0xB0); // write pointer command code
  Wire.write(0x8F); // write the register to point to
  Wire.endTransmission();
  // tell AD5933 to expect a block read of 1 byte
  Wire.beginTransmission(0x0D);
  Wire.write(0xA1); // block read command
  Wire.write(0x01); // number of bytes to read
  Wire.endTransmission();
  // read 1 byte from AD5933
  Wire.requestFrom(0x0D, 1);
  // store byte
  firstByte = Wire.read();

To read the temperature register of the AD5933
AD5933 Command:
  block read 2 bytes from register 0x92
ESP32 Command:
  // first set the AD5933's pointer to register 0x92
  Wire.beginTransmission(0x0D);
  Wire.write(0xB0); // write pointer command code
  Wire.write(0x92); // write the register to point to
  Wire.endTransmission();
  // tell AD5933 to expect a block read of 2 bytes
  Wire.beginTransmission(0x0D);
  Wire.write(0xA1); // block read command
  Wire.write(0x02); // number of bytes to read
  Wire.endTransmission();
  // read 2 bytes from AD5933
  Wire.requestFrom(0x0D, 2);
  // store bytes
  firstByte = Wire.read();
  secondByte = Wire.read();

To read the real and imaginary data from the impedance measurement
AD5933 Command:
  block read 4 bytes from register 0x94
ESP32 Command:
  // first set the AD5933's pointer to register 0x94
  Wire.beginTransmission(0x0D);
  Wire.write(0xB0); // write pointer command code
  Wire.write(0x94); // write the register to point to
  Wire.endTransmission();
  // tell AD5933 to expect a block read of 4 bytes
  Wire.beginTransmission(0x0D);
  Wire.write(0xA1); // block read command
  Wire.write(0x04); // number of bytes to read
  Wire.endTransmission();
  // read 4 bytes from AD5933
  Wire.requestFrom(0x0D, 4);
  // store bytes
  firstByte = Wire.read();
  secondByte = Wire.read();
  thirdByte = Wire.read();
  fourthByte = Wire.read();
  */
