'''
Author: Thirawat Bureetes
Email: tbureete@purdue.edu
Date: 5/27/2021
Description: Database model file.
'''

from datetime import datetime
from flask_sqlalchemy import SQLAlchemy

db = SQLAlchemy()

class Sweep(db.Model):
    __tablename__ = 'sweeps'
    
    id = db.Column(db.Integer, primary_key=True)
    device_name = db.Column(db.String())
    filename = db.Column(db.String)
    sensor_time = db.Column(db.DateTime(timezone=True), default=None)
    hub_time = db.Column(db.DateTime(timezone=True), default=None)
    server_time = db.Column(db.DateTime(timezone=True))
    temperature = db.Column(db.Integer, default=None)
    rssi = db.Column(db.Integer, default=None)

    def __init__(self, device_name, filename, sensor_time=None, hub_time=None, temperature=None, rssi=None):
        self.device_name = device_name
        self.filename = filename
        self.sensor_time = sensor_time
        self.hub_time = hub_time
        self.server_time = datetime.utcnow().replace(microsecond=0)
        self.temperature = temperature
        self.rssi = rssi

    def __repr__(self):
        return f'Sweep from {self.device_name} received at {self.server_time}.'

    def json(self):
        return {
            'filename': self.filename,
            'device_name': self.device_name,
            'timestamp': self.server_time.isoformat()
        }
