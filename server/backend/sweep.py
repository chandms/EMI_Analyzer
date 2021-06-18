'''
Author: Thirawat Bureetes
Email: tbureete@purdue.edu
Date: 5/27/2021
Description: Sweep database API.
'''

from flask_restful  import Resource, reqparse
from models import Sweep, Device
from datetime import timezone

class SweepAPI(Resource):

    def get(self):
        parser = reqparse.RequestParser()
        parser.add_argument('latest')
        args = parser.parse_args()
        sweeps = []

        if args['latest'] is not None:
            device_query = Device.query.order_by(Device.last_updated.desc()).all()
            sweep_query = []
            for device in device_query:
                sweep_query.append(Sweep.query.filter_by(device_id=device.id).order_by(Sweep.hub_time.desc()).first())            

        else:
            sweep_query = Sweep.query.order_by(Sweep.hub_time.desc()).all()

        for sweep in sweep_query:
            sweeps.append({
                'id': sweep.id,
                'device_name': sweep.device.name,
                'hub_timestamp': sweep.hub_time.replace(tzinfo=timezone.utc).isoformat(),
                'server_timestamp': sweep.server_time.replace(tzinfo=timezone.utc).isoformat(),
                'rssi': sweep.rssi,
                'filename': sweep.filename
                })

        return sweeps
