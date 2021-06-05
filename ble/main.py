'''
Author: Thirawat Bureetes
Email: tbureete@purdue.edu
Date: 6/02/2021
Description: The main script to connect to ble devices.
'''
import re
import asyncio
from pandas import DataFrame
from bleak.backends.device import BLEDevice

from connect import scan_devices, auto_connect

def detect_device() -> BLEDevice:
    '''
        Scan all devices. Try to detect the proper device. Otherwise, return None.
    '''
    devices = scan_devices()
    for device in devices:
        if re.match(r'(EMI)', device.name):
            return device
    
    return None


if __name__ == '__main__':

    device = detect_device()
    if device is not None:
        print(f'Connecting to {device.name} ({device.address}) ...')
        meta_data, sweep = asyncio.run(auto_connect(device))
        sweep_df = DataFrame.from_dict(sweep)
        print(sweep_df)

