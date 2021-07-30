'''
Author: Thirawat Bureetes
Email: tbureete@purdue.edu
Date: 5/27/2021
Description: Sweep database API.
'''

import yaml
from pathlib import Path
from flask_restful  import Resource, reqparse
from models import db, Sweep, Device
from analyzer import strength_calculator
from log import logger

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

        if args['device_name'] is not None:
            device_name = args['device_name']
            device_query = Device.query.filter_by(name=device_name).first()
            sweep_query = Sweep.query.filter_by(device_id=device_query.id).order_by(Sweep.hub_time.desc()).all()


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

            sweeps.append(sweep_data)

        return sweeps
