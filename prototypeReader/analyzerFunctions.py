import serial
# sends the current sweep over usb
def send_sweep(sweep):
    # create a serial connection
    ser = serial.Serial()

    # try to connect to port COM5
    try:
        ser.port = 'COM5'
    except:
        # for some reason I couldn't put the code below here so I just
        print('')

    if not (ser.is_open):
        print('USB connection could not be established. Aborting')
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
             s - send the sweep to the sensor
             x - execute the sweep on the sensor (must send the sweep with "s" first)''')
