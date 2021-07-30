'''
Author: Thirawat Bureetes
Email: tbureete@purdue.edu
Date: 06/10/2021
Description: API for manage sweep download.
'''

import yaml
from flask import send_file
from flask_restful  import Resource, reqparse
from pathlib import Path
from models import Sweep
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
            return send_file(filepath, as_attachment=True)
        
        else: 
            logger.error(f'sweep id {sweep_id} is not found.')
            return {'message': f'sweep id {sweep_id} is not found'}, 400
