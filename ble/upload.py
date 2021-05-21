import requests
import yaml
from pathlib import Path
from random import choice

def upload_sweep(sweep, uri):
    with open(sweep, 'rb') as f:
        files = {'file': f}
        r = requests.post(uri, files=files)
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