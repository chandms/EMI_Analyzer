import requests
from pathlib import Path
from random import choice

url = 'http://10.165.76.237:50000/upload/'

def upload_sweep(sweep):
    with open(sweep, 'rb') as f:
        files = {'file': f}
        r = requests.post(url, files=files)
        return r

if __name__ == '__main__':

    path = Path('/home/pi/Desktop/EMI/sweeps')
    sweeps = []
    for x in path.iterdir():
        sweeps.append(x)

    sweep = choice(sweeps)
    r = upload_sweep(sweep)
    print(r.status_code)
    print(r.text)