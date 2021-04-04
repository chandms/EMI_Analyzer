import asyncio
import array
from bleak import BleakScanner
from bleak import discover
from bleak import BleakClient

ADDRESS = 'C6:3F:7F:8E:A7:D1'
UART_NORDIC = '6E400001-B5A3-F393-E0A9-E50E24DCCA9E'
UUID_NORDIC_RX = '6E400003-B5A3-F393-E0A9-E50E24DCCA9E'
UUID_NORDIC_TX = '6E400002-B5A3-F393-E0A9-E50E24DCCA9E'

command = 'Do you hear me?'

# async def run():
#     devices = await BleakScanner.discover()
#     for d in devices:
#         print(d)

# loop = asyncio.get_event_loop()
# loop.run_until_complete(run())

def uart_data_received(sender, raw_data):
    print(f'RX> ({len(raw_data)} Bytes) {raw_data.decode()}')

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


loop = asyncio.get_event_loop()
loop.run_until_complete(run(ADDRESS, loop))