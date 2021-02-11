import serial

def execute_sweep():
    # open usb connection and check if success
    ser = open_usb()
    if not (ser):
        return

    # send the execute sweep command
    buff = [3]
    buff = bytes(buff)
    ser.write(buff)

    # should read 3 back
    buff = ser.read(1)
    if (int.from_bytes(buff, "little") == 3):
        print("Sweep Started")
    else:
        return

    freq = [0] # stores the frequency data
    imp = [0, 0] # stores the impedance data 0:real 1:imaginary

    # get data from the sweep, if the current frequency is 3, then the sweep is done
    while (freq != 3):
        # read a data point from the sensor
        buff = ser.read(8)
        
        # if the frequency of the data point is 0 (which is not possible on the AD5933) then the sweep is done
        freq = int.from_bytes(buff[:4], "little", signed=False)
        if (freq == 0):
            break

        # get real and imaginary data and convert it
        imp[0] = int.from_bytes(buff[4:6], "little", signed=True)
        imp[1] = int.from_bytes(imp[6:], "little", signed=True)

        print(f'Frequency: {freq} Real: {imp[0]} Imaginary: {imp[1]}')

    print("Sweep Complete")

# sends the current sweep over usb
def send_sweep(sweep):
    # open a usb connection
    ser = open_usb()

    # check if connection success
    if not (ser):
        return

    # convert cyclesMultiplier, range, clockSource, and gain to values understood by AD5933
    converted = [0,0,0,0] # cyclesMultiplier, range, clockSource, gain
    
    # convert cyclesMultiplier
    if (sweep.get('cyclesMultiplier') == 1):
        converted[0] = 0
    elif (sweep.get('cyclesMultiplier') == 2):
        converted[0] = 1
    elif (sweep.get('cyclesMultiplier') == 4):
        converted[0] = 3

    # convert range
    if (sweep.get('range') == 1):
        converted[1] = 0
    elif (sweep.get('range') == 2):
        converted[1] = 3
    elif (sweep.get('range') == 3):
        converted[1] = 2
    elif (sweep.get('range') == 4):
        converted[1] = 1

    # convert clockSource
    if (sweep.get('clockSource') == 'internal'):
        converted[2] = 0
    elif (sweep.get('clockSource') == 'external'):
        converted[2] = 1

    # convert gain
    if (sweep.get('gain') == 1):
        converted[3] = 1
    elif (sweep.get('gain') == 5):
        converted[3] = 0
    
    # write a 2 for sweep transfer
    buff = [2]
    buff = bytes(buff)
    ser.write(buff)

    # expect a 2 in return
    buff = ser.read(2)
    if (buff.decode('ascii') == 'cs'):
       buff = struct.pack('8I 8I 2I 2I 1I 1I 1I 8I 1I',
               sweep.get('start'),
               sweep.get('delta'),
               sweep.get('steps'),
               sweep.get('cycles'),
               converted[0],
               converted[1],
               converted[2],
               sweep.get('clockFrequency'),
               converted[3])

       # write the data
       ser.write(buff)

       # expect a 'c' back
       buff = ser.read(1)
       if (buff.decode('ascii') == 'c'):
           print('Sweep set successfully')
           return

    print('Sweep send failed')

# opens a usb connection on COM5
def open_usb():
    # create a serial connection
    ser = serial.Serial()

    # try to connect to port COM5
    try:
        ser.port = 'COM5'
        ser.timout = 10
        ser.write_timeout = 10
        ser.open()
    except:
        # for some reason I couldn't put the code below here so I just
        print('')

    if not (ser.is_open):
        print('USB connection failed')
        return False
    else:
        return ser

# checks if the prototype is connected over usb
def check_usb():
    # create a serial connection
    ser = serial.Serial()

    # try to connect to port COM5
    try:
        ser.port = 'COM5'
        ser.timout = 10
        ser.write_timout = 10
        ser.open()
    except:
        # for some reason I couldn't put the code below here so I just
        print('')

    if not (ser.is_open):
        print('COM port not open. Make sure the device is plugged in')
    else:
        # send a 1 to test connection
        send = [1]
        send = bytes(send)
        ser.write(send)

        # device should return 1
        recieve = ser.read(1)

        if (int.from_bytes(recieve, "little") == 1):
            print('Device Connected')
        else:
            print('Device Not Connected')

    return

# edits the current sweep
def edit_sweep(sweep):
    # each parameter only has a certain range so we have to do this the long way
    print('Hit ENTER to keep current value')

    start = input('Start Frequency (Hz): ')
    if (start != ''):
        start = int(start)
        while ((start < 1000) or (start > 100000)):
            print('Start Frequency must be within 1kHz to 100kHz')
            start = int(input('Start Frequency (Hz): '))
        sweep['start'] = start

    delta = input('Delta Frequency (Hz): ')
    if (delta != ''):
        delta = int(delta)
        while ((delta < 0) or (delta > 100000)):
            print('Delta Frequency must be within 0Hz to 100kHz')
            delta = int(input('Delta Frequency (Hz): '))
        sweep['delta'] = delta

    steps = input('Number of Increments: ')
    if (steps != ''):
        steps = int(steps)
        while ((steps < 0) or (steps > 511)):
            print('Increments must be within 0 to 511')
            steps = int(input('Number of Increments: '))
        sweep['steps'] = steps

    cycles = input('Number of Settling Time Cycles: ')
    if (cycles != ''):
        cycles = int(cycles)
        while ((cycles < 0) or (cycles > 511)):
            print('Time Cycles must be within 0 to 511')
            cycles = int(input('Number of Settling Time Cycles: '))
        sweep['cycles'] = cycles

    mult = input('Time Cycles Multiplier (1, 2, 4): ')
    if (mult != ''):
        mult = int(mult)
        while (not (mult in [1, 2, 4])):
            print('Multiplier must be 1, 2, or 4')
            mult = int(input('Time Cycles Multiplier: '))
        sweep['cyclesMultiplier'] = mult
    
    rang = input('Output Excitation Range (1, 2, 3, 4): ')
    if (rang != ''):
        rang = int(rang)
        while (not (rang in [1, 2, 3, 4])):
            print('Range must be 1, 2, 3, or 4')
            rang = int(input('Output Excitation Range: '))
        sweep['range'] = rang

    source = input('Clock Source ("internal" or "external"): ')
    if (source != ''):
        while not ((source == 'internal') or (source == 'external')):
            print('Clock Source must be "internal" or "external"')
            source = input('Clock Source: ')
        sweep['clockSource'] = source

    if (source == 'internal'):
        print('Setting Clock Frequency to AD5933 internal clock 16776000 Hz')
        sweep['clockFrequency'] = 16776000
    elif (source == 'external'):
        sweep['clockFrequency'] = int(input('Input the external clock frequency: '))

    gain = input('PGA Gain (1 or 5): ')
    if (gain != ''):
        gain = int(gain)
        while (not (gain in [1, 5])):
            print('PGA Gain must be 1 or 5')
            gain = int(input('PGA Gain: '))
        sweep['gain'] = gain
    
    print('Sweep Edit Complete')
    return sweep

# prints the current sweep to the console
def print_sweep(sweep):
    print(f'''Sweep Parameters
            Start Frequency: {sweep.get('start')} Hz
            Delta Frequency: {sweep.get('delta')} Hz
            Number of Increments: {sweep.get('steps')}
            Number of Settling Time Cycles: {sweep.get('cycles')}
            Time Cycles Multiplier: X{sweep.get('cyclesMultiplier')}
            Output Excitation Range: Range {sweep.get('range')}
            Clock Source: {sweep.get('clockSource')}
            Clock Frequency: {sweep.get('clockFrequency')} Hz
            PGA Gain: X{sweep.get('gain')}''')

# prints the valid commands
def print_commands():
    print('''Valid Commands:
             q - quit the program
             p - print the current sweep
             e - edit the current sweep
             c - check if device is connected
             s - send the sweep to the sensor
             x - execute the sweep on the sensor (must send the sweep with "s" first)''')
