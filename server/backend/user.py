'''
Author: Thirawat Bureetes
Email: tbureete@purdue.edu
Date: 8/24/2021
Description: API for authenticate user using jwt
'''

import jwt
from dotenv import dotenv_values
from datetime import datetime, timezone, timedelta
from flask_restful  import Resource, reqparse
from models import User
from log import logger

env_config = dotenv_values()

class LoginAPI(Resource):

    def get(self):
        return {'message': 'If you see this, it means the authentication api is online!'}

    def post(self):
        parser = reqparse.RequestParser()
        parser.add_argument('username', required=True)
        parser.add_argument('password', required=True)
        args = parser.parse_args()
        user = User.query.filter_by(username=args['username']).first()

        if user is None:
            return {'message': f'The username {args["username"]} ' \
                               f'does not exist in the database'}, 400
        
        else:
            if user.password == args["password"]:
                create_time = datetime.now(timezone.utc).replace(microsecond=0)
                payload = {
                    'iat': create_time,
                    'exp': create_time + timedelta(hours=1),
                    'firstname': user.firstname,
                    'lastname': user.lastname,
                    'email': user.email
                }
                token = jwt.encode(
                    payload,
                    env_config["secret"],
                    algorithm='HS256'
                ).decode('utf8')
                
                return token

            else:
                return {'message': f'Invalid password'}, 400

