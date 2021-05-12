from flask_restx import Resource, reqparse
from werkzeug.datastructures import FileStorage

# upload_parser = api.parser()
# upload_parser.add_argument('file', location='files',
#                            type=FileStorage, required=True)

class UploadHandler(Resource):
    def get(self, filename):
        return {'filename': filename}
    
    def post(self, filename):
        parser = reqparse.RequestParser()
        parser.add_argument('foo', type=int)
        # parser.add_argument('filepath', type=FileStorage, location='files')
        args = parser.parse_args()
        print(args)
        return {'filename': filename}