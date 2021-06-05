'''
Author: Thirawat Bureetes
Email: tbureete@purdue.edu
Date: 6/02/2021
Description: The main script to connect to ble devices.
'''
import re
import asyncio
from bleak import BleakClient
from bleak.backends.device import BLEDevice
from logging import raiseExceptions
from connect import scan_devices, transfer_data
from nordic import save_sweep

def detect_device() -> BLEDevice:
    '''
        Scan all devices. Try to detect the proper device. Otherwise, return None.
    '''
    devices = scan_devices()
    for device in devices:
        if re.match(r'(EMI)', device.name):
            return device
    
    return None

async def connect(device: BLEDevice) -> BleakClient:
    '''
        Connect to the BLE device
    '''
    connection = BleakClient(device)
    try:
        print(f'Connecting to {device.name} ({device.address}) ...')
        await connection.connect()
        return connection
    
    except TimeoutError: 
        raiseExceptions(asyncio.TimeoutError)

    except Exception as e:
        print(e)

async def automate(device: BLEDevice):
    '''
        The main connection and transfer data routine.
    '''
    try:
        connection = await asyncio.wait_for(connect(device), timeout=15)
        meta_data, sweep = await asyncio.wait_for(transfer_data(connection), timeout=15)
        meta_data.rssi = device.rssi
        meta_data.device_name = device.name
        save_sweep(meta_data, sweep)

    except asyncio.TimeoutError:
        print('Timeout!. Something went wrong')

    except Exception as e:
        print(e)
    
    finally:
        print('Disconnecting ...')
        await connection.disconnect()
    

if __name__ == '__main__':

    device = detect_device()
    if device is not None:
        asyncio.run(automate(device))
    else:
        print('No devices found.')

        

