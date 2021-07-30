'''
Author: Henry Silva, Thirawat Bureetes
Email: tbureete@purdue.edu
Date: 7/29/2021
Description: Calculate concrete's strength from sweep data.
'''

import pandas as pd
from data_process.calcFunctions import calc_gain_factor, calc_impedance
from data_process.strengthCalculator import get_strength
from pathlib import Path

path = Path('/home/tam/git/EMI/server/backend')
default_profile = path / 'data_process' / 'default_profile.csv'

def strength_calculator(sweep_file, output_path, gain_profile=default_profile, cali:int=22):
    '''
    Input parameters:
        sweep_file is input sweep
        output_path is a directory and filename of the output file
        gain_profile: gain 
        cali is calibration impedance       
    '''
    # read the gain file and convert to tuples
    gain = pd.read_csv(gain_profile)
    gain = list(gain.itertuples(index=False, name=None))
    
    # calculate the gain factors 
    # (for now it is assumed that the calibration resistance is 1k Ohm)
    gain = calc_gain_factor(gain, cali)

    # Calcualte impedance for each sweep and save to Output folder
    data = list(pd.read_csv(sweep_file).itertuples(index=False,name=None))
    data = calc_impedance(data, gain)
    data = pd.DataFrame(data, columns=['Frequency', 'Impedance', 'Phase', 'Conductance', 'Susceptance'])
    data.to_csv(output_path, index=False)

    # Calculate the strength for the sweep
    strength = get_strength(data)

    return strength

if __name__ == '__main__':
    demo_file = path / 'data_process' / 'EMI_JEO-2021-07-22T174555.csv'
    print(impedance_calculator(sweep_file=demo_file, output_path='test.csv'))
