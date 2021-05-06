import datetime
from pathlib import Path
import pandas as pd
import asyncio
import array
from bleak import BleakScanner
from bleak import discover
from bleak import BleakClient

# Tam DK
ADDRESS = 'C6:3F:7F:8E:A7:D1' 
# Henry DK
# ADDRESS = 'F2:F3:78:24:DF:44'
UART_NORDIC = '6E400001-B5A3-F393-E0A9-E50E24DCCA9E'
UUID_NORDIC_RX = '6E400003-B5A3-F393-E0A9-E50E24DCCA9E'
UUID_NORDIC_TX = '6E400002-B5A3-F393-E0A9-E50E24DCCA9E'

command = 'Do you hear me?'

class MetaData():
    def __init__(self):
        self.n_freq = 0
        self.temperature = 0
        self.time = 0
    
    def __str__(self):
        return f'Number of frequency data = {self.n_freq} \n' \
               f'Time = {self.time} \n' \
               f'temperature = {self.temperature}'
                

# async def run():
#     devices = await BleakScanner.discover()
#     for d in devices:
#         print(d)

# loop = asyncio.get_event_loop()
# loop.run_until_complete(run())

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

print('Connecting...')
async def run(ADDRESS, loop):
    async with BleakClient(ADDRESS, loop=loop) as client:
        print('Connected')
        await client.start_notify(UUID_NORDIC_RX, uart_data_received)
        # print('Writing command')
        # c=command
        # while len(c)>0:
        #   await client.write_gatt_char(UUID_NORDIC_TX, bytearray(c[0:10]), True)
        #   c = c[20:]
        print('Waiting for data')
        await asyncio.sleep(10.0, loop=loop) # wait for a response
        print('Done!')

meta_data = MetaData()
sweep = []
loop = asyncio.get_event_loop()
loop.run_until_complete(run(ADDRESS, loop))
print(meta_data)
print(f'Got {len(sweep)} data')
sweep_df = pd.DataFrame.from_dict(sweep)
print(sweep_df)
path = Path('/home/pi/Desktop/EMI/sweeps')
filename = path / f'{datetime.datetime.now().replace(microsecond=0).isoformat()}.csv'
sweep_df.to_csv(filename, index=False)
print(f'Save to {filename}')