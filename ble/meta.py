'''
Author: Thirawat Bureetes
Email: tbureete@purdue.edu
Date: 06/05/2021
Description: Define structure for meta data.
'''

class MetaData():
    '''
        Sweep meta data structure
    '''
    def __init__(self):
        self.n_freq = 0
        self.temperature = 0
        self.time = 0
        self.device_name = ''
        self.mac_addres = ''
        self.rssi = 0
        self.hub_timestamp = 0
    
    def __str__(self):
        return f'Device name = {self.device_name} \n' \
               f'MAC Address = {self.mac_addres} \n' \
               f'RSSI = {self.rssi} \n' \
               f'Number of frequency data = {self.n_freq} \n' \
               f'Time = {self.time} \n' \
               f'temperature = {self.temperature}'
               