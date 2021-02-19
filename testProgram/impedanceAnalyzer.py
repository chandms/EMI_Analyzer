import analyzerFunctions as af
import pandas as pd
# panda is a data analysis library

# create the default sweep dictionary to store the sweep parameters
sweep = {
        'start': 1000,
        'delta': 100,
        'steps': 490,
        'cycles': 15,
        'cyclesMultiplier': 1,
        'range': 1,
        'clockSource': 'internal',
        'clockFrequency': 16776000,
        'gain': 1
        }

# create DataFrame to store the data (blank at start)
df = pd.DataFrame()

# variables to store program state
gotGain = False
gotData = False

# the number of sweeps to average when calculating gain/impedance
num_ave = 1

# let the user know that h can be used to get command
print('This is the default sweep:')
af.print_sweep(sweep)
print('For a list of commands enter "h"')

# main loop for getting user input
while(1):
    cmd = input('Input a Command: ')

    if (cmd == 'q'):
        quit()
    elif (cmd == 'c'):
        af.check_usb()
    elif (cmd == 'h'):
        af.print_commands()
    elif (cmd == 'p'):
        af.print_sweep(sweep)
    elif (cmd == 'e'):
        sweep = af.edit_sweep(sweep)
        af.print_sweep(sweep)
    elif (cmd == 's'):
        af.send_sweep(sweep)
    elif (cmd == 'a'):
        num_ave = af.set_ave()
    elif (cmd == 'g'):
        gain = af.get_gain(num_ave)
        gotGain = True
    elif (cmd == 'x'):
        if gotGain:
            df = af.sweep_ave(gain, num_ave)
            gotData = True
            print(df)
            print('Sweep Complete')
        else:
            print('Calculate Gain Factor First!')
    elif (cmd == 'o'):
        if gotData:
            af.output_csv(df)
        else:
            print('Execute a Sweep First!')
    else:
        print('Input a valid command!')
    print('')
