import pandas as pd
import scipy.signal as ss
import matplotlib.pyplot as plt

def modulus_disc(freq, gainFactor):
    # some variables for calculations
    miu  = 0.25
    dens = 2.3e3
    
    # the calculations
    cpp   = freq * gainFactor
    c_miu = (1 + miu) * (1 - (2 * miu)) / (1 - miu)
    yd    = (cpp ** 2) * dens * c_miu * (1e-9)
    young = 446.09 * (yd ** 1.4) / dens
    stren = (young * 1e3 / 0.043 / (dens ** 1.5)) ** 2

    return stren

# takes the dataFrame from the impedance caluclations and finds the strength
# the upper and lower bounds of frequency can be optionally specified
def get_strength(data, lower=20000, upper=50000):
    G = []
    F = []
    pks = []
    pkf = []

    # only include conductance points within the freq range
    for ind in data.index:
        if (data['Frequency'][ind] >= lower and data['Frequency'][ind] <= upper):
            G.append(data['Conductance'][ind])
            F.append(data['Frequency'][ind])

    # smooth the data
    Gs = ss.savgol_filter(G,11,1)

    # find the peaks
    pks, _ = ss.find_peaks(Gs, height=0.001)

    #plt.plot(F,Gs)

    for pk in pks:
    #    plt.axvline(F[pk], color='r', lw=0.75)
        pkf.append(F[pk])

    #plt.show()

    # get the strength
    stren = modulus_disc(pkf[0], 0.130)

    return stren


if __name__ == "__main__":
    data = pd.read_csv('CAI_CH09_0719.csv')
    print(f'Strength is: {get_strength(data)} MPa')
