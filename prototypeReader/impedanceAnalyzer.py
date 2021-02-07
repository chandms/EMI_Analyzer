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
df = pd.DataFrame([0])

# let the user know that h can be used to get command
print('For a list of commands enter "h"')

# main loop for getting user input
while(1):
    cmd = input('Input a Command: ')

    if (cmd == 'q'):
        quit()
    elif (cmd == 'h'):
        af.print_commands()
    elif (cmd == 'p'):
        af.print_sweep(sweep)
    elif (cmd == 'e'):
        sweep = af.edit_sweep(sweep)
        af.print_sweep(sweep)
    else:
        print('Input a valid command!')


