'''
Author: Thirawat Bureetes
Email: tbureete@purdue.edu
Date: 07/11/2021
Description: device database API.
'''

from flask_restful  import Resource, reqparse
from models import db, Device
from log import logger

class DeviceAPI(Resource):

    def get(self):
        parser = reqparse.RequestParser()
        parser.add_argument('device_name')
        args = parser.parse_args()

        if args['device_name'] is not None:
            device_name = args['device_name']
            device = Device.query.filter_by(name=device_name).first()

            if device is not None:
                return device.json()

            else:
                return {
                    'message': f'device {device_name} is not found.'
                }, 400
        
        else:
            devices = []
            device_query = Device.query.order_by(Device.last_updated.desc()).all()
            for device in device_query:
                devices.append(device.json())
            
            return devices

    def post(self):
        parser = reqparse.RequestParser()
        parser.add_argument('device_id', required=True)
        parser.add_argument('longitude', required=True)
        parser.add_argument('latitude', required=True)
        args = parser.parse_args()

        device = Device.query.filter_by(id=args['device_id']).first()
        device.longitude = args['longitude']
        device.latitude = args['latitude']
        logger.info(f'{device.name} updated location on database.')
        db.session.commit()

        return {
            'longitude': args['longitude'],
            'latitude': args['latitude']
        }, 201
