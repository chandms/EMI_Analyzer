'''
Author: Thirawat Bureetes
Email: tbureete@purdue.edu
Date: 5/27/2021
Description: This is function for uploading file to the server.
'''

import requests
from requests.models import Response
import yaml
import re
from pathlib import Path, PosixPath
from random import choice

def upload_sweep(sweep: PosixPath, uri: str) -> Response:
    '''
        Upload sweep to the server. Pass a path to sweep file and destination url.
        Return post request response.
    '''
    group = re.findall(r'(.*)-(\d{4}-\d{2}-\d{2}T.{8})', sweep.stem)[0]
    body = {
        'device_name': group[0],
        'hub_time': group[1]
    }
    with open(sweep, 'rb') as f:
        files = {'file': f}
        r = requests.post(uri, files=files, data=body)
        return r


if __name__ == '__main__':

    with open('config.yaml') as f:
        configs = yaml.load(f, Loader=yaml.FullLoader)
    
    path = Path(configs['sweep_path'])
    sweeps = []
    for x in path.iterdir():
        sweeps.append(x)
    sweep = choice(sweeps)
    uri = configs['upload_uri'][configs['env']]
    print(f'Uploading sweep file to {uri}')
    r = upload_sweep(sweep, uri)
    print(f'Status code = {r.status_code}')
    print(r.text)