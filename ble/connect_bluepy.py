from bluepy.btle import Scanner, DefaultDelegate


ADDRESS = 'C6:3F:7F:8E:A7:D1'
UART_NORDIC = '6E400001-B5A3-F393-E0A9-E50E24DCCA9E'
UUID_NORDIC_RX = '6E400003-B5A3-F393-E0A9-E50E24DCCA9E'
UUID_NORDIC_TX = '6E400002-B5A3-F393-E0A9-E50E24DCCA9E'

class ScanDelegate(DefaultDelegate):
    def __init__(self):
        DefaultDelegate.__init__(self)

    def handleDiscovery(self, dev, isNewDev, isNewData):
        if isNewDev:
            print("Discovered device", dev.addr)
        elif isNewData:
            print("Received new data from", dev.addr)

scanner = Scanner().withDelegate(ScanDelegate())
devices = scanner.scan(10.0)

for dev in devices:
    print(f'Device {dev.addr} ({dev.addrType}), RSSI={dev.rssi} dB')
    for (adtype, desc, value) in dev.getScanData():
        print(f'{desc} = {value}')


# print("Connecting...")
# dev = btle.Peripheral(ADDRESS)
 
# print("Services...")
# for svc in dev.services:
#     print(str(svc))