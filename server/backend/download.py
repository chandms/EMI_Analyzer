'''
Author: Thirawat Bureetes
Email: tbureete@purdue.edu
Date: 06/10/2021
Description: API for manage sweep download.
'''

import yaml
import pandas as pd
from flask import send_file
from flask_restful  import Resource, reqparse
from pytz import timezone
from pathlib import Path
from models import Sweep, Device
from log import logger

with open('config.yaml') as f:
    configs = yaml.load(f, Loader=yaml.FullLoader)
    path = Path(configs['sweep_path'])

class DownloadHandler(Resource):

    def get(self):
        parser = reqparse.RequestParser()
        parser.add_argument('id', required=True)
        args = parser.parse_args()
        sweep_id = args['id']
        sweep = Sweep.query.filter(Sweep.id == sweep_id).first()
        logger.info(f'Request for sweep id {sweep_id}.')
        if sweep is not None:
            filepath = path / sweep.filename
            download_name = f'{sweep.devices.name}-' \
                            f'{sweep.sensor_time.astimezone(timezone("US/East-Indiana")).isoformat()[:-6]}.csv'
            return send_file(filepath, 
                             as_attachment=True, 
                             download_name=download_name,
                             mimetype='application/x-csv')
        
        else: 
            logger.error(f'sweep id {sweep_id} is not found.')
            return {'message': f'sweep id {sweep_id} is not found'}, 400


class ParameterDownloader(Resource):

    def get(self):
        parser = reqparse.RequestParser()
        parser.add_argument('device_name', required=True)
        args = parser.parse_args()
        device_name = args['device_name']
        device = Device.query.filter(Device.name == device_name).first()
        if device is not None:
            data = []
            for sweep in device.sweeps:
                data.append(sweep.json())
            df = pd.DataFrame(data)
            filename = f'{device_name}_parameters.csv'
            filepath = Path('/tmp') / filename
            df.to_csv(filepath, index=False)
            return send_file(filepath, 
                            as_attachment=True, 
                            download_name=filename,
                            mimetype='application/x-csv')       
        else: 
            logger.error(f'Device {device_name} is not found.')
            return {'message': f'Device {device_name} is not found'}, 400
