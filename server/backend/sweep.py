'''
Author: Thirawat Bureetes
Email: tbureete@purdue.edu
Date: 5/27/2021
Description: Sweep database API.
'''

from datetime import datetime
from flask_restful  import Resource, reqparse
from models import db, Sweep

class SweepAPI(Resource):

    def get(self):
        sweeps = []
        for sweep in Sweep.query.order_by(Sweep.server_time.desc()).all():
            sweeps.append({
                'id': sweep.id,
                'filename': sweep.filename,
                'device_name': sweep.device_name
                })
        return sweeps