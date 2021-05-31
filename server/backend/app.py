'''
Author: Thirawat Bureetes
Email: tbureete@purdue.edu
Date: 5/27/2021
Description: The main script for backend server.
'''

from flask import Flask
from flask_restful  import Api
from flask_cors import CORS
from dotenv import dotenv_values

from models import db
from upload import UploadHandler
from sweep import SweepAPI

env_config = dotenv_values()

app = Flask(__name__)
CORS(app)
app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False
app.config["SQLALCHEMY_DATABASE_URI"] = 'postgresql://' \
                                        f'{env_config["db_user"]}:' \
                                        f'{env_config["db_pass"]}@' \
                                        f'{env_config["db_domain"]}:' \
                                        f'{env_config["db_port"]}/' \
                                        f'{env_config["db_table"]}'
db.init_app(app)
api = Api(app)

api.add_resource(UploadHandler, '/upload')
api.add_resource(SweepAPI, '/sweep')

if __name__ == '__main__':
    app.run(host='0.0.0.0', debug=True)