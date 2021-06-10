'''
Author: Thirawat Bureetes
Email: tbureete@purdue.edu
Date: 5/27/2021
Description: Sweep database API.
'''

from flask_restful  import Resource, reqparse
from models import db, Sweep
from datetime import timezone

class SweepAPI(Resource):

    def get(self):
        parser = reqparse.RequestParser()
        parser.add_argument('latest')
        args = parser.parse_args()
        sweeps = []
        if args['latest'] is not None:
            sweep_query = db.session.query(Sweep).all()
        else:
            sweep_query = Sweep.query.order_by(Sweep.server_time.desc()).all()

        for sweep in sweep_query:
            sweeps.append({
                'id': sweep.id,
                'device_name': sweep.device_name,
                'hub_timestamp': sweep.hub_time.replace(tzinfo=timezone.utc).isoformat(),
                'server_timestamp': sweep.server_time.replace(tzinfo=timezone.utc).isoformat(),
                'rssi': sweep.rssi
                })

        return sweeps
