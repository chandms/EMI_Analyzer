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
from models import db, Sweep



with open('config.yaml') as f:
    configs = yaml.load(f, Loader=yaml.FullLoader)
    path = Path(configs['sweep_path'])

class UploadHandler(Resource):

    def get(self):
        return {'message': 'If you see this, it means the system works!'}

    def post(self):
        parser = reqparse.RequestParser()
        parser.add_argument('file', type=FileStorage, location='files', required=True)
        parser.add_argument('device_name', required=True)
        parser.add_argument('hub_time', required=True)
        parser.add_argument('rssi')
        args = parser.parse_args()
        upload_file = args['file']
        filename = secure_filename(upload_file.filename)
        upload_file.save(path / filename)
        sweep_meta = Sweep(
            filename = filename,
            device_name = args['device_name'],
            hub_time = datetime.fromisoformat(args['hub_time']),
            rssi = args['rssi']
        )
        db.session.add(sweep_meta)
        db.session.commit()
        
        return sweep_meta.json(), 201
