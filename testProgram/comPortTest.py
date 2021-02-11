import serial
import struct

# this program reads 5 bytes from COM5 and then stops
# run this program from Windows command line or it wont work

ser = serial.Serial()
ser.port = 'COM5'
ser.timout = 10
ser.write_timeout = 10
ser.open()

bytesRead = 0

data = [63]
data = bytes(data)
ser.write(data)
print(f'Sending: {data}')

while (bytesRead < 9):
    data = ser.read(1)
    print(data)
    bytesRead += 1
