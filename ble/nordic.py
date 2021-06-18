'''
Author: Thirawat Bureetes
Email: tbureete@purdue.edu
Date: 6/03/2021
Description: Library for Nordic ralated.
'''

import yaml
from pandas import DataFrame, read_csv
from pathlib import Path
from datetime import datetime, timezone
from upload import upload_sweep
from meta import MetaData
from log import logger

UART_NORDIC = '6E400001-B5A3-F393-E0A9-E50E24DCCA9E'
UUID_NORDIC_RX = '6E400003-B5A3-F393-E0A9-E50E24DCCA9E'
UUID_NORDIC_TX = '6E400002-B5A3-F393-E0A9-E50E24DCCA9E'

with open('config.yaml') as f:
    configs = yaml.load(f, Loader=yaml.FullLoader)

def save_sweep(meta_data: MetaData, sweeps: list) -> None:
    '''
        Save sweeps into local directory and try to upload.
    '''
    sweep_df = DataFrame.from_dict(sweeps)
    path = Path(configs['sweep_path'])
    timestamp = datetime.now(timezone.utc).replace(microsecond=0).isoformat()[:-6]
    filename = path / f'{meta_data.device_name}-{timestamp}.csv'
    sweep_df.to_csv(filename, index=False)
    meta_data.hub_timestamp = timestamp
    meta_file = path / 'meta.csv'
    logger.info(f'Recorded sweeps into {filename}.')
    uri = configs['upload_uri'][configs['env']]
    r = upload_sweep(filename, uri, meta_data)
    if r.status_code == 201:
        logger.info(f'Uploaded to {uri} completed.')
        if meta_file.is_file():
            meta_df = read_csv(meta_file)
            if len(meta_df.index) > 0:
                logger.info(f'Found {len(meta_df.index)} unsent sweeps.')
                for index, meta in meta_df.iterrows():
                    meta_data = MetaData()
                    meta_data.device_name = meta.device_name
                    meta_data.hub_timestamp =meta.hub_time
                    meta_data.rssi = meta.rssi
                    meta_data.temperature = meta.temperature
                    meta_data.time = meta.sensor_time
                    meta_data.mac_addres = meta.mac_address
                    filename = path / f'{meta_data.device_name}-{meta_data.hub_timestamp}.csv'
                    r = upload_sweep(filename, uri, meta_data)
                    if r.status_code == 201:
                        logger.info(f'Uploaded sweep from {meta_data.device_name} at {meta_data.hub_timestamp} completed.')
                        meta_df = meta_df.drop([index])

                meta_df.to_csv(meta_file, index=False)
    else:
        logger.warning(f'Uploaded to {uri} failed. Save file locally.')
        new_meta = {
            'device_name': meta_data.device_name,
            'hub_time': meta_data.hub_timestamp,
            'rssi': meta_data.rssi,
            'temperature': meta_data.temperature,
            'sensor_time': meta_data.time,
            'mac_address': meta_data.mac_addres
        }
        if meta_file.is_file():
            meta_df = read_csv(meta_file)
            meta_df = meta_df.append(new_meta, ignore_index=True)
        else:            
            meta_df = DataFrame(new_meta, index=[0])

        meta_df.to_csv(meta_file, index=False)




