# EMI Analyzer

This repository contains the project files for the EMI Analyzer developed for Purdue University's [SMART Lab](https://engineering.purdue.edu/SMARTLab).

## nRF52 Code

The code located in the prototypeCode folder is for the nRF52840 Dev Board. The NRF SDK and Keil are required to compile and load this code to the dev board. Replace the Keil project files and SDK config file of an example project located in the peripherals examples in the SDK. Then copy the main.c and AD5933.c/.h files where main.c is located in that example.

## Python Script

There is a Python script located in the testProgram folder that can control and get impedance data from the AD5933 connected to the NRF52 dev board. Run the script from the command prompt. You can install the necessary libraries with pip. Currently, the send sweep command does not work and a sweep must be preloaded onto the dev board.

