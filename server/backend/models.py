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
    __tablename__ = 'sweeps'
    
    id = db.Column(db.Integer, primary_key=True)
    device_id = db.Column(db.Integer(), db.ForeignKey('devices.id'))
    filename = db.Column(db.String)
    sensor_time = db.Column(db.DateTime(timezone=True), default=None)
    hub_time = db.Column(db.DateTime(timezone=True), default=None)
    server_time = db.Column(db.DateTime(timezone=True))
    temperature = db.Column(db.Integer, default=None)
    rssi = db.Column(db.Integer, default=None)
    strength = db.Column(db.Integer, default=None)

    def __init__(self, filename, sensor_time, hub_time, temperature, rssi=None):
        self.filename = filename
        self.sensor_time = sensor_time
        self.hub_time = hub_time
        self.server_time = datetime.now(timezone.utc).replace(microsecond=0)
        self.temperature = temperature
        self.rssi = rssi

    def __repr__(self):
        return f'Sweep from {self.devices.name} received at {self.server_time}'

    def json(self):
        return {
            'id': self.id,
            'device_name': self.devices.name,
            'device_id': self.device_id,
            'sensor_timestamp': self.sensor_time.isoformat() if self.sensor_time is not None 
                                else self.hub_time.isoformat(),
            'hub_timestamp': self.hub_time.isoformat(),
            'server_timestamp': self.server_time.isoformat(),
            'rssi': self.rssi,
            'strength': self.strength,
            'temperature': self.temperature
        }

class Device(db.Model):
    __tablename__ = 'devices'

    id = db.Column(db.Integer, primary_key=True)
    name = db.Column(db.String())
    mac_address = db.Column(db.String())
    last_updated = db.Column(db.DateTime(timezone=True))
    longitude = db.Column(db.Float)
    latitude = db.Column(db.Float)
    sweeps = db.relationship('Sweep', backref='devices')

    def __init__(self, name, timestamp, mac_address=None) -> None:
        self.name = name
        self.mac_address = mac_address
        self.last_updated = timestamp

    def __repr__(self) -> str:
        return f'Device {self.name} last update at {self.last_updated}'

    def json(self):
        return {
            'device_id': self.id,
            'device_name': self.name,
            'mac_address': self.mac_address,
            'last_updated': self.last_updated.isoformat(),
            'longitude': self.longitude,
            'latitude': self.latitude
        }

class User(db.Model):
    __tablename__ = 'users'

    id = db.Column(db.Integer, primary_key=True)
    username = db.Column(db.String())
    password = db.Column(db.String())
    firstname = db.Column(db.String())
    lastname = db.Column(db.String())
    email = db.Column(db.String())
    last_login = db.Column(db.DateTime(timezone=True))
    # project = db.Column(db.Integer, db.ForeignKey('projects.id'))

    def __init__(self, username, password, firstname, lastname, email=None) -> None:
        self.username = username
        self.password = password
        self.firstname = firstname
        self.lastname = lastname
        self.email = email

    def __repr__(self) -> str:
        return f'User {self.firstname} {self.lastname} last seen at {self.last_login}'


# class Project(db.Model):
#     __tablename__ = 'projects'

#     id = db.Column(db.Integer, primary_key=True)
#     name = db.Column(db.String())
#     hubs = db.relationship('SensorHub', backref='projects')
#     users = db.relationship('User', backref='projects')

#     def __init__(self, name) -> None:
#         self.name = name

# class SensorHub(db.Model):
#     __tablename__ = 'hubs'

#     id = db.Column(db.Integer, primary_key=True)
#     name = db.Column(db.String())
#     devices = db.relationship('Device', backref='hubs')

#     def __init__(self, name) -> None:
#         self.name = name

