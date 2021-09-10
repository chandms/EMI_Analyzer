'''
Author: Thirawat Bureetes
Email: tbureete@purdue.edu
Date: 5/27/2021
Description: Sweep database API.
'''

import yaml
from datetime import datetime
from pathlib import Path
from flask_restful  import Resource, reqparse
from models import db, Sweep, Device
from analyzer import strength_calculator
from log import logger
import numpy as np
import pandas as pd

with open('config.yaml') as f:
    configs = yaml.load(f, Loader=yaml.FullLoader)
    path = Path(configs['sweep_path'])

class SweepAPI(Resource):

    def get(self):
        parser = reqparse.RequestParser()
        parser.add_argument('latest')
        parser.add_argument('device_name')
        args = parser.parse_args()
        sweeps = []
        strengths = []

        if args['device_name'] is not None:
            device_name = args['device_name']
            device_query = Device.query.filter_by(name=device_name).first()
            sweep_query = Sweep.query.filter_by(device_id=device_query.id).order_by(Sweep.hub_time.desc()).all()

            
            for index, sweep in enumerate(sweep_query):
                if sweep.strength is not None and sweep.strength > 0:
                    strengths.append({'x': index, 'y': sweep.strength})

            if len(strengths) > 0:
                str_df = pd.DataFrame(strengths)
                A = np.vstack([str_df['x'], np.ones(len(str_df['x']))]).T
                m, c = np.linalg.lstsq(A, str_df['y'], rcond=None)[0]
                str_df = str_df.set_index('y')

        elif args['latest'] is not None:
            device_query = Device.query.order_by(Device.last_updated.desc()).all()
            sweep_query = []
            for device in device_query:
                sweep_query.append(Sweep.query.filter_by(device_id=device.id).order_by(Sweep.hub_time.desc()).first())            

        else:
            sweep_query = Sweep.query.order_by(Sweep.hub_time.desc()).all()

        for sweep in sweep_query:
            sweep_data = sweep.json()

            if sweep_data['strength'] is None:
                input_file = path / sweep.filename
                output_file = path / f'{input_file.stem}-processed.csv'
                try:
                    strength = strength_calculator(input_file, output_file)

                except Exception as e:
                    logger.error(f'Sweep {input_file.stem} has no impededance value => {e}')
                    strength = 0
            
                sweep.strength = strength
                db.session.commit()
                
            if len(strengths) > 0 and sweep.strength > 0:
                sweep_data['trend'] = round(str_df.loc[sweep.strength, 'x']*m + c, 2)
            else: 
                sweep_data['trend'] = None

            sweeps.append(sweep_data)

        return sweeps

class SweepManual(Resource):

    def post(self):
        parser = reqparse.RequestParser()
        parser.add_argument('device_name', required=True)
        parser.add_argument('sensor_time', required=True)
        parser.add_argument('temperature')
        parser.add_argument('ambient_temp')
        parser.add_argument('strength')
        args = parser.parse_args()
        sensor_timestamp = datetime.fromisoformat(args['sensor_time'])
        hub_timestamp = sensor_timestamp
        # check if the device is already in the database or not
        device = Device.query.filter_by(name=args['device_name']).first()
        if device is None:
            logger.info(f'Device is not found. Creating a new one.')
            device = Device(
                name = args['device_name'],
                timestamp = sensor_timestamp
            )
            db.session.add(device)

        else:
            logger.info(f'{device} found.')
            if device.last_updated < sensor_timestamp:
                device.last_updated = sensor_timestamp
        
        # save file
        sweep_meta = Sweep(
            filename = '#',
            hub_time = hub_timestamp,
            sensor_time = sensor_timestamp,
            temperature = float(args['temperature']) if (args['temperature']) is not None
                          else None
        )
        sweep_meta.ambient_temp = float(args['ambient_temp']) if (args['ambient_temp']) is not None \
                                  else None
        sweep_meta.strength = float(args['strength'])
        device.sweeps.append(sweep_meta)
        db.session.add(sweep_meta)
        logger.info(f'{sweep_meta} recorded into database.')
        db.session.commit()
      
        return sweep_meta.json(), 201
