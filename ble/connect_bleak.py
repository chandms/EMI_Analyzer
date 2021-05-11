import datetime
from pathlib import Path
import pandas as pd
import asyncio
from bleak import BleakScanner
from bleak import BleakClient

UART_NORDIC = '6E400001-B5A3-F393-E0A9-E50E24DCCA9E'
UUID_NORDIC_RX = '6E400003-B5A3-F393-E0A9-E50E24DCCA9E'
UUID_NORDIC_TX = '6E400002-B5A3-F393-E0A9-E50E24DCCA9E'

class MetaData():
    def __init__(self):
        self.n_freq = 0
        self.temperature = 0
        self.time = 0
    
    def __str__(self):
        return f'Number of frequency data = {self.n_freq} \n' \
               f'Time = {self.time} \n' \
               f'temperature = {self.temperature}'

def uart_data_received(sender, raw_data):
    # print(f'RX> ({len(raw_data)} Bytes) {raw_data.decode()}')
    # print(f'RX> Recieved {len(raw_data)} Bytes')
    message_type = raw_data[0]
    if message_type == 0: # Meta Data
        print(f'Meta Data recieved')
        meta_data.n_freq = int.from_bytes(raw_data[1:5], byteorder='little', signed=False)
        meta_data.time = int.from_bytes(raw_data[5:9], byteorder='little', signed=False)
        meta_data.temperature = int.from_bytes(raw_data[9:11], byteorder='little', signed=False)
    
    else:
        freq_got = int(message_type)
        print(f'Number of frequency = {freq_got}')
        print(f'RX> Recieved {len(raw_data)} Bytes')
        for i in range(freq_got):
            base_index = i*8
            freq = int.from_bytes(raw_data[base_index+1:base_index+5], byteorder='little', signed=False)
            real = int.from_bytes(raw_data[base_index+5:base_index+7], byteorder='little', signed=False)
            imag = int.from_bytes(raw_data[base_index+7:base_index+9], byteorder='little', signed=False)
            sweep.append({
                'freq': freq, 
                'real': real, 
                'imag': imag
            })

def connection_command():
    print(f'Select following commands')
    print(f'{"q": >2}: Terminate connection')
    print(f'{"m": >2}: Get metadata')
    print(f'{"g": >2}: Get sweep')
    command = input()
    return command

async def scan(time=5):
    devices = await BleakScanner.discover(time)
    return devices

async def connect(device):
    async with BleakClient(device) as connection:
        while not connection.is_connected:
            pass
        print('Connected')
        await connection.start_notify(UUID_NORDIC_RX, uart_data_received)
        command = connection_command()
        while(command != 'q'):
            if command == 'm':
                await connection.write_gatt_char(UUID_NORDIC_TX, bytearray('0', 'utf-8'), True)
                print(f'There are {meta_data.n_freq} frequencies')
            elif command == 'g':
                while len(sweep) < meta_data.n_freq:
                    await connection.write_gatt_char(UUID_NORDIC_TX, bytearray('1', 'utf-8'), True)
                    await asyncio.sleep(0.1)
            command = connection_command()
        print('Disconnecting ...')

def scan_devices():
    print('Scaning BLE devices ...')
    devices = asyncio.run(scan())
    print(f'Found {len(devices)} devices')
    print(f' # {"name":>20} {"RSSI":>8}')
    print('-'*40)
    for index, device in enumerate(devices):
        print(f'{index+1:2} {device.name:>20} {device.rssi:>4} dBm')
    print('-'*40)
    return devices

devices = scan_devices()
selected = int(input('Select a BLE device: '))
while selected == 0:
    devices = scan_devices()
    selected = int(input('Select a BLE device: '))
device = devices[selected-1]
print(f'Connecting to {device.name} ({device.address}) ...')

meta_data = MetaData()
sweep = []
connection = asyncio.run(connect(device))
print(f'Got {len(sweep)} data')
sweep_df = pd.DataFrame.from_dict(sweep)
print(sweep_df)
path = Path('/home/pi/Desktop/EMI/sweeps')
filename = path / f'{datetime.datetime.now().replace(microsecond=0).isoformat()}.csv'
sweep_df.to_csv(filename, index=False)
print(f'Save to {filename}')
