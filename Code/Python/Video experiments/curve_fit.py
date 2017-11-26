from plot_helper import read_movement_data
import math as m
import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit


def func(x, a, b, c, d):
    return a*x**3 + b*x**2 + c*x + d

ddir = 'Csv_Data/'
mdir = 'Video_Straight_4s/'

plt.figure(figsize=(5, 5))
cnt = 1
while cnt < 3:
    data = read_movement_data(ddir+mdir+'Video_trg_50_r1_battery_int200_'+str(cnt)+'.csv')
    x, y = zip(*data)
    xdata = np.asarray(x)
    ydata = np.asarray(y)


    plt.plot(xdata, ydata)

    popt, pcov = curve_fit(func, xdata, ydata)
    plt.plot(xdata, func(xdata, *popt), 'r-', label='fit: a=%5.3f, b=%5.3f, c=%5.3f, d=%5.3f' % tuple(popt))

    plt.xlim(xmin=-10, xmax=90)
    plt.ylim(ymin=-10, ymax=90)
    # plt.title(file_name)
    plt.xlabel('x distance in cm')
    plt.ylabel('y distance in cm')
    plt.legend()
    #plt.axes().set_aspect('equal', 'datalim')
    cnt += 1
plt.show()

print data

