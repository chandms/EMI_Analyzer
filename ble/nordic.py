'''
Author: Thirawat Bureetes
Email: tbureete@purdue.edu
Date: 6/03/2021
Description: Library for Nordic ralated.
'''

UART_NORDIC = '6E400001-B5A3-F393-E0A9-E50E24DCCA9E'
UUID_NORDIC_RX = '6E400003-B5A3-F393-E0A9-E50E24DCCA9E'
UUID_NORDIC_TX = '6E400002-B5A3-F393-E0A9-E50E24DCCA9E'

class MetaData():
    def __init__(self):
        self.n_freq = 0
        self.temperature = 0
        self.time = 0
        self.device_name = ''
        self.rssi = 0
    
    def __str__(self):
        return f'Device name = {self.device_name} \n' \
               f'RSSI = {self.rssi} \n' \
               f'Number of frequency data = {self.n_freq} \n' \
               f'Time = {self.time} \n' \
               f'temperature = {self.temperature}'

# def save_sweep(meta_data, sweeps):
#     sweep_df = DataFrame.from_dict(sweep)
#     print(sweep_df)
#     path = Path(configs['sweep_path'])
#     filename = path / f'{device.name}-{datetime.now(timezone.utc).replace(microsecond=0).isoformat()[:-6]}.csv'
#     sweep_df.to_csv(filename, index=False)