from flask_restx import Resource, reqparse
from werkzeug.utils import secure_filename
from werkzeug.datastructures import FileStorage
from pathlib import Path

path = Path('/home/tam/git/EMI/sweeps')

class UploadHandler(Resource):

      def post(self):
        parser = reqparse.RequestParser()
        parser.add_argument('file', type=FileStorage, location='files', required=True)
        args = parser.parse_args()
        upload_file = args['file']
        filename = secure_filename(upload_file.filename)
        upload_file.save(path / filename)
        return {'filename': filename}