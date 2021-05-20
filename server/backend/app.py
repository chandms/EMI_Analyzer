from flask import Flask
from flask_restful  import Api
from flask_cors import CORS
from upload import UploadHandler

app = Flask(__name__)
CORS(app)
api = Api(app)
api.add_resource(UploadHandler, '/upload/')

if __name__ == '__main__':
    app.run(host='0.0.0.0', debug=True)