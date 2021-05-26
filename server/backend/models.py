import datetime
from flask_sqlalchemy import SQLAlchemy

db = SQLAlchemy()

class Sweep(db.Model):
    __tablename__ = 'sweeps'
    
    id = db.Column(db.Integer, primary_key=True)
    device_name = db.Column(db.String())
    sensor_time = db.Column(db.DateTime, default=None)
    hub_time = db.Column(db.DateTime, default=None)
    server_time = db.Column(db.DateTime, default=datetime.datetime.now)
    filename = db.Column(db.String)

    def __init__(self, device_name, filename, sensor_time=None, hub_time=None):
        self.device_name = device_name
        self.filename = filename
        self.sensor_time = sensor_time
        self.hub_time = hub_time

    def __repr__(self):
        return f'Device {self.device_name} received at {self.server_time}'

    def json(self):
        return {
            'id': self.id,
            'device_name': self.device_name,
            'sensor_time':self.sensor_time,
            'hub_time': self.hub_time,
            'server_time': self.server_time,
            'filename': self.filename
        }