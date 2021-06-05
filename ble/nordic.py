'''
Author: Thirawat Bureetes
Email: tbureete@purdue.edu
Date: 6/03/2021
Description: Library for Nordic ralated.
'''

import yaml
from pandas import DataFrame
from pathlib import Path
from datetime import datetime, timezone
from upload import upload_sweep
from meta import MetaData

UART_NORDIC = '6E400001-B5A3-F393-E0A9-E50E24DCCA9E'
UUID_NORDIC_RX = '6E400003-B5A3-F393-E0A9-E50E24DCCA9E'
UUID_NORDIC_TX = '6E400002-B5A3-F393-E0A9-E50E24DCCA9E'

with open('config.yaml') as f:
    configs = yaml.load(f, Loader=yaml.FullLoader)

def save_sweep(meta_data: MetaData, sweeps: list):
    '''
        Save sweeps into local directory.
    '''
    sweep_df = DataFrame.from_dict(sweeps)
    path = Path(configs['sweep_path'])
    filename = path / f'{meta_data.device_name}-{datetime.now(timezone.utc).replace(microsecond=0).isoformat()[:-6]}.csv'
    sweep_df.to_csv(filename, index=False)
    uri = configs['upload_uri'][configs['env']]
    r = upload_sweep(filename, uri, meta_data)

    return r
    