from __future__ import division
from plot_helper import *
import math as m
import numpy as np
import matplotlib.pyplot as plt


def length_of_movement(num_samp, dlist):
    dlist.reverse()
    print 'max xy: ' + str(m.hypot(dlist[len(dlist)-1][0] - dlist[0][0], dlist[len(dlist)-1][1] - dlist[0][1]))
    cnt = 1
    cnt2 = 0
    length = 0
    while cnt < len(dlist)-1:
        if dlist[cnt][1] > dlist[cnt2][1]:
            length += m.hypot(dlist[cnt][0] - dlist[cnt2][0], dlist[cnt][1] - dlist[cnt2][1])
            cnt2 += (cnt-cnt2)
        cnt += 1
    #print 'length: ' + str(length)
    return length


def lom_multi():
    lcnt = 1
    result = []
    while lcnt < 3:
        data = read_movement_data(ddir + mdir + file_name + str(lcnt) + '.csv')
        result.append(length_of_movement(300, data))
        lcnt += 1
    fix = np.asarray(result)
    print 'length mean: ' + str(np.mean(fix))
    print 'length std: ' + str(np.std(fix))
    print 'length max: ' + str(np.max(fix))


def plt_dist_straight(num_samp, data_list):
    bp = data_list[0]
    ep = data_list[len(data_list)-1]
    x = np.linspace(bp[0], ep[0], num_samp)
    y = np.linspace(bp[1], ep[1], num_samp)
    cnt = 0
    data = []
    while cnt < num_samp:
        cnt2 = 0
        sdist = 80
        while cnt2 < len(data_list)-1:
            test = m.hypot(data_list[cnt2][0] - x[cnt], data_list[cnt2][1] - y[cnt])
            if test < sdist:
                sdist = test
            cnt2 += 1
        data.append(sdist)
        cnt += 1
    plt.plot(data)
    return data, abs(ep[0]-bp[0])


def plot_dist_multi():
    plt.figure()
    lcnt = 1
    mean = []
    std = []
    max = []
    hd = []
    while lcnt < 3:
        data = read_movement_data(ddir + mdir + file_name + str(lcnt) + '.csv')
        result, hdv = plt_dist_straight(100, data)
        npdata = np.asarray(result)
        mean.append(np.mean(npdata))
        std.append(np.std(npdata))
        max.append(np.max(npdata))
        hd.append(hdv)
        lcnt += 1
    print 'avg mean:' + str(np.mean(mean))
    print 'avg std: ' + str(np.mean(std))
    print 'max: ' + str(np.max(max))
    print 'horizontal deviation: ' + str(np.mean(hd))


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

    #print 'x =' + str(x)
    #print 'y =' + str(y)
    #plt.plot(x, y, linewidth=2, color='black', linestyle='--')
    plt.xlim(xmin=-10, xmax=90)
    plt.ylim(ymin=-10, ymax=90)
    # plt.title(file_name)
    plt.xlabel('x distance in cm')
    plt.ylabel('y distance in cm')
    #plt.axes().set_aspect('equal', 'datalim')
    #plt.legend()


ddir = 'Csv_Data/'
mdir = 'Video_Straight/'
#mdir = 'Video_Solar/'
file_name = 'Video_trg_70_r1_75_battery_int500_'
#file_name = 'Video_straight_trg_30_r1_75_solar_'

lom_multi()

plot_raw_data()

#plot_dist_multi()

plt.show()

