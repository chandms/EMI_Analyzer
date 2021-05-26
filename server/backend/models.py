import datetime
from flask_sqlalchemy import SQLAlchemy

db = SQLAlchemy()

class Sweep(db.Model):
    __tablename__ = 'sweeps'
    
    id = db.Column(db.Integer, primary_key=True)
    device_name = db.Column(db.String())
    filename = db.Column(db.String)
    sensor_time = db.Column(db.DateTime, default=None)
    hub_time = db.Column(db.DateTime, default=None)
    server_time = db.Column(db.DateTime, default=datetime.datetime.now)

    def __init__(self, device_name, filename, sensor_time=None, hub_time=None):
        self.device_name = device_name
        self.filename = filename
        self.sensor_time = sensor_time
        self.hub_time = hub_time

    def __repr__(self):
        return f'Sweep from {self.device_name} received.'

    def json(self):
        return {
            'filename': self.filename,
            'device_name': self.device_name,
        }