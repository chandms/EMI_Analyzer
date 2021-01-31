import serial
from pynput import keyboard
import time

# this program reads 100 lines from COM5 and then stops
# run this program from Windows command line or it wont work

ser = serial.Serial('COM5')

linesRead = 0

while (linesRead < 100):
    data = ser.readline()
    print(data)
    linesRead += 1
