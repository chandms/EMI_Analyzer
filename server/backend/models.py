'''
Author: Thirawat Bureetes
Email: tbureete@purdue.edu
Date: 5/27/2021
Description: Database model file.
'''

from datetime import datetime, timezone
from flask_sqlalchemy import SQLAlchemy

db = SQLAlchemy()

class Sweep(db.Model):
    __tablename__ = 'sweep'
    
    id = db.Column(db.Integer, primary_key=True)
    device_id = db.Column(db.Integer(), db.ForeignKey('device.id'))
    filename = db.Column(db.String)
    sensor_time = db.Column(db.DateTime(timezone=True), default=None)
    hub_time = db.Column(db.DateTime(timezone=True), default=None)
    server_time = db.Column(db.DateTime(timezone=True))
    temperature = db.Column(db.Integer, default=None)
    rssi = db.Column(db.Integer, default=None)

    def __init__(self, filename, sensor_time=None, hub_time=None, temperature=None, rssi=None):
        self.filename = filename
        self.sensor_time = sensor_time
        self.hub_time = hub_time
        self.server_time = datetime.now(timezone.utc).replace(microsecond=0)
        self.temperature = temperature
        self.rssi = rssi

    def __repr__(self):
        return f'Sweep from {self.device.name} received at {self.server_time}'

    def json(self):
        return {
            'filename': self.filename,
            'device_name': self.device.name,
            'timestamp': self.server_time.isoformat()
        }

class Device(db.Model):
    __tablename__ = 'device'

    id = db.Column(db.Integer, primary_key=True)
    name = db.Column(db.String())
    mac_address = db.Column(db.String())
    last_updated = db.Column(db.DateTime(timezone=True))
    longitude = db.Column(db.Float)
    latitude = db.Column(db.Float)
    sweeps = db.relationship('Sweep', backref='device')

    def __init__(self, name, timestamp, mac_address=None) -> None:
        self.name = name
        self.mac_address = mac_address
        self.last_updated = timestamp

    def __repr__(self) -> str:
        return f'Device {self.name} last update at {self.last_updated}'

    def json(self):
        return {
            'device_name': self.name,
            'mac_address': self.mac_address,
            'last_updated': self.last_updated
        }
