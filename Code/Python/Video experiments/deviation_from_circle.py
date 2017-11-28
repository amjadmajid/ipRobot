from __future__ import division
from plot_helper import *
import math as m
import numpy as np
import matplotlib.pyplot as plt


def plt_dist_circle(num_samp, data_list):
    theta = np.linspace(np.pi, -np.pi, num_samp)
    #r = 30
    #xoff = -1
    #yoff = -7
    x = 5.80 + r + xoff + (r * np.cos(theta))
    y = 9.02 + r + yoff + (r * np.sin(theta))

    cnt = 0
    data = []
    while cnt < num_samp:
        cnt2 = 0
        sdist = r
        while cnt2 < len(data_list)-1:
            test = m.hypot(data_list[cnt2][0] - x[cnt], data_list[cnt2][1] - y[cnt])
            if test < sdist:
                sdist = test
            cnt2 += 1
        data.append(sdist)
        cnt += 1
    plt.plot(data)
    return data


def plot_dist_multi():
    plt.figure()
    lcnt = 1
    mean = []
    std = []
    while lcnt < 3:
        data = read_movement_data(ddir + mdir + file_name + str(lcnt) + '.csv')
        result = plt_dist_circle(100, data)
        npdata = np.asarray(result)
        mean.append(np.mean(npdata))
        std.append(np.std(npdata))
        lcnt += 1
    print 'avg mean:' + str(mean[0] + mean[1] / 2)
    print 'avg std: ' + str(std[0] + std[1] / 2)



def plot_raw_data():
    plt.figure(figsize=(5, 5))
    cnt = 1
    while cnt < 3:
        data = read_movement_data(ddir+mdir+file_name+str(cnt)+'.csv')
        #print len(data)
        #print length_of_movement(data, 10)
        x, y = zip(*data)
        xdata = np.asarray(x)
        ydata = np.asarray(y)
        #print 'xdata: ' + str(xdata)
        #print 'ydata: ' + str(ydata)
        plt.plot(xdata, ydata)

        cnt += 1

    theta = np.linspace(np.pi, -np.pi, 200)
    x = 5.8 + r + xoff + (r * np.cos(theta))
    y = 9.0 + r + yoff + (r * np.sin(theta))
    #print 'x =' + str(x)
    #print 'y =' + str(y)
    plt.plot(x, y, linewidth=2, color='black', linestyle='--')
    plt.xlim(xmin=-10, xmax=90)
    plt.ylim(ymin=-10, ymax=90)
    # plt.title(file_name)
    plt.xlabel('x distance in cm')
    plt.ylabel('y distance in cm')
    #plt.axes().set_aspect('equal', 'datalim')
    #plt.legend()


ddir = 'Csv_Data/'
mdir = 'Video_Circle/'
#file_name = 'Video_trg_30_r1_right_r30_battery_'
file_name = 'Video_trg_50_r1_right_r30_battery_'

r = 34
xoff = -1
yoff = -7
plot_raw_data()

plot_dist_multi()

plt.show()