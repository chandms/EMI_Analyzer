'''
Author: Thirawat Bureetes
Email: tbureete@purdue.edu
Date: 5/27/2021
Description: API for manage sweep uploading.
'''

import yaml
from datetime import datetime
from flask_restful  import Resource, reqparse
from werkzeug.utils import secure_filename
from werkzeug.datastructures import FileStorage
from pathlib import Path
from models import db, Sweep, Device
from log import logger


with open('config.yaml') as f:
    configs = yaml.load(f, Loader=yaml.FullLoader)
    path = Path(configs['sweep_path'])

class UploadHandler(Resource):

    def get(self):
        return {'message': 'If you see this, it means the upload api is online!'}

    def post(self):
        parser = reqparse.RequestParser()
        parser.add_argument('file', type=FileStorage, location='files', required=True)
        parser.add_argument('device_name', required=True)
        parser.add_argument('mac_address', required=True)
        parser.add_argument('hub_time', required=True)
        parser.add_argument('sensor_time', required=True)
        parser.add_argument('temperature', required=True)
        parser.add_argument('rssi')
        args = parser.parse_args()
        hub_timestamp = datetime.fromisoformat(args['hub_time'])
        sensor_timestamp = datetime.fromisoformat(args['sensor_time'])
        # check if the device is already in the database or not
        device = Device.query.filter_by(name=args['device_name'],
                                        mac_address=args['mac_address']).first()
        if device is None:
            logger.info(f'Device is not found. Creating a new one.')
            device = Device(
                name = args['device_name'],
                mac_address = args['mac_address'],
                timestamp = sensor_timestamp
            )
            db.session.add(device)

        else:
            logger.info(f'{device} found.')
            if device.last_updated < sensor_timestamp:
                device.last_updated = sensor_timestamp
        
        # save file
        upload_file = args['file']
        filename = secure_filename(upload_file.filename)
        upload_file.save(path / filename)
        sweep_meta = Sweep(
            filename = filename,
            hub_time = hub_timestamp,
            rssi = args['rssi'],
            sensor_time = sensor_timestamp,
            temperature = args['temperature']
        )
        device.sweeps.append(sweep_meta)
        db.session.add(sweep_meta)
        logger.info(f'{sweep_meta} recorded into database.')
        db.session.commit()
      
        return sweep_meta.json(), 201
