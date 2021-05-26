from flask import Flask
from flask_restful  import Api
from flask_cors import CORS


from models import db
from upload import UploadHandler
from sweep import SweepAPI

app = Flask(__name__)
CORS(app)
app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False
app.config["SQLALCHEMY_DATABASE_URI"] = 'postgresql://postgres:1n1t1al0s@10.165.76.237:5432/emi'
db.init_app(app)
api = Api(app)



api.add_resource(UploadHandler, '/upload')
api.add_resource(SweepAPI, '/sweep')

if __name__ == '__main__':
    app.run(host='0.0.0.0', debug=True)