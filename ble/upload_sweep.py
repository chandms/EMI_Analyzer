import requests
from pathlib import Path

path = Path('/home/pi/Desktop/EMI/sweeps')
 
sweeps = []
for x in path.iterdir():
    sweeps.append(x)

sweep = sweeps[3]
print(sweep)
url = 'http://10.165.76.237:50000/upload/'
with open(sweep, 'rb') as f:
    files = {'file': f}
    r = requests.post(url, files=files)
    print(r.status_code)
    print(r.text)