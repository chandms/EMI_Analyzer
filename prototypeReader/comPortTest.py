import serial

# this program reads 5 bytes from COM5 and then stops
# run this program from Windows command line or it wont work

ser = serial.Serial('COM5')
ser.timout = None

bytesRead = 0

while (bytesRead < 5):
    data = [bytesRead]
    data = bytes(data)
    ser.write(data)
    data = ser.read(1)
    print(data)
    bytesRead += 1
