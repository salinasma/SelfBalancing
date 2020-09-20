import matplotlib.pyplot as plt
import sympy as sp
from scipy import stats
from scipy import fftpack
from scipy import signal 
import numpy as np

import sys
import os
import csv

time = np.array([])
yValue = np.array([])

T = 1.0 / 1000.0

def data_averaging( raw_data=[] , sample_divider=1):
    averaged_data = np.array([])
    if  raw_data == [] or sample_divider == 1:
        return raw_data

    else:
        aggregator = 0
        aggregator_counter = 0
        for data in raw_data:
            if aggregator_counter < sample_divider: 
                aggregator += data
                aggregator_counter += 1
            else:
                averaged_data = np.append(averaged_data , aggregator/aggregator_counter )
                aggregator_counter = 0
                aggregator = 0
        
        if aggregator_counter != 0: 
            averaged_data = np.append(averaged_data , aggregator/aggregator_counter )
        return  averaged_data

def butter_worth( gain =1, order:int = 2, cutOffFrequency_hz =10):
    omega_c_rads_ps = cutOffFrequency_hz * 2 * np.pi


    num, den = signal.butter(order, omega_c_rads_ps, analog=False)
    return  num, den



def load_csv_data( file_name:str, sample_period):
    x = np.array([])
    y = np.array([])
    with open (file_name) as csvData: 
        entireFile = csv.reader(csvData, delimiter=',')

        entireFile.__next__()
        for row in entireFile:
            if len(row) > 0:
                x = np.append( x, float(row[0]) * sample_period )
                y = np.append( y, float(row[1]) )
    
    T= sample_period

    return x, y


def main():
    print("")

if __name__ == "__main__":
    main()

