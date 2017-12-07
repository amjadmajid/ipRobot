from plot_helper import *
import os
import numpy as np
from collections import OrderedDict
import matplotlib.pyplot as plt


def read_csv_data_to_list(csv_list, re_list):
    data_list = []
    lcnt = 0
    re_cnt = 0
    while lcnt < len(csv_list):
        if re.match(file_name + re_list[re_cnt] + '_\d', csv_list[lcnt], flags=0) is not None:
            print file_name + re_list[re_cnt]

            slist = read_movement_data(ddir + mdir + csv_list[lcnt])
            tmp_list = [slist, re_cnt]
            data_list.append(tmp_list)

        else:
            if re_cnt < (len(re_list) - 1):
                re_cnt += 1
                lcnt -= 1
            else:
                re_cnt = 0

        lcnt += 1
    return data_list


def plt_distance_from_ref_circle(data_list):

    samples = 300
    theta = np.linspace(-np.pi, np.pi, samples)
    r = 30
    x = 5.8 + r + (r * np.cos(theta))
    y = 9.0 + r + (r * np.sin(theta))

    plt.figure()
    for dlist in data_list:
        jump = int(round(len(dlist[0])/samples))
        cnt = 0
        jcnt = 0
        data = []
        while cnt < samples:
            data.append(m.hypot(dlist[0][jcnt][0] - x[cnt], dlist[0][jcnt][1] - y[cnt]))
            cnt += 1
            jcnt += jump
        plt.plot(data, label=label[dlist[1]], color=color_list[dlist[1]], marker=marker_list[dlist[1]], markevery=20, alpha=0.5)

    # remove duplicate labels
    handles, labels = plt.gca().get_legend_handles_labels()
    by_label = OrderedDict(zip(labels, handles))
    plt.legend(by_label.values(), by_label.keys(), loc='upper right')
    plt.show()


def plot_raw_data(data_list):

    if movement == 1:
        plt.figure(figsize=(5, 5))
    elif movement == 4:
        plt.figure(figsize=(3, 5))
        plt.subplots_adjust(left=0.2)
    for dlist in data_list:
        plt.plot(*zip(*dlist[0]), label=label[dlist[1]], color=color_list[dlist[1]], marker=marker_list[dlist[1]], markevery=20, alpha=0.5)

    if movement == 1:
        theta = np.linspace(np.pi, -np.pi, 200)
        x = 36 + 30 * np.cos(theta)
        y = 38 + 30 * np.sin(theta)
        plt.plot(x, y, linewidth=2, color='black', linestyle='--')
    if movement == 3:
        plt.vlines(x=39, ymin=1, ymax=76, color='black', linewidth=2)
        plt.hlines(y=1, xmin=32, xmax=46, color='black', linewidth=2)
        plt.hlines(y=76, xmin=32, xmax=46, color='black', linewidth=2)
    plt.xlim(xmin=-10, xmax=90)
    plt.ylim(ymin=-10, ymax=90)
    # plt.title(file_name)
    plt.xlabel('x distance in cm', fontsize=12)
    plt.ylabel('y distance in cm', fontsize=12)
    plt.xticks(fontsize=12)
    plt.yticks(fontsize=12)
    if movement == 4:
        plt.xlim(xmin=20, xmax=80)  # adjust the max leaving min unchanged
    #plt.axes().set_aspect('equal', 'datalim')
    # remove duplicate labels
    handles, labels = plt.gca().get_legend_handles_labels()
    by_label = OrderedDict(zip(labels, handles))
    plt.legend(by_label.values(), by_label.keys(), loc='upper right')
    plt.show()




ddir = 'Csv_Data/'

movement = 1
set = 2
pwm = ['30', '50', '70']
#re_list = ['', '_int500', '_int750', '_int1000', '_int1250']
re_list = ['battery', 'battery_int500', 'battery_int1000', 'solar']
if movement == 1:
    mdir = 'Video_Circle/'
    file_name = 'Video_trg_' + pwm[set] + '_r1_right_r30_'
elif movement == 2:
    mdir = 'Video_Square/'
    file_name = 'Video_trg_70_r1_right_battery'
elif movement == 3:
    mdir = 'Video_Straight/'
    file_name = 'Video_trg_' + pwm[set] + '_r1_75_'
elif movement == 4:
    mdir = 'Video_Straight_4s/'
    file_name = 'Video_trg_' + pwm[set] + '_r1_battery'
    re_list = ['_int200', '_int300', '_int400']

csv_list = os.listdir(ddir+mdir)
csv_list.sort(key=natural_keys)

if movement != 4:
    # label = ['No Interrupt', 'Interrupt 0.5s', 'Interrupt 0.75s', 'Interrupt 1.0s', 'Interrupt 1.25s']
    label = ['No Interrupt', 'Interrupt 0.5s', 'Interrupt 1.0s', 'Solar']
else:
    label = ['Interrupt 0.2s', 'Interrupt 0.3s', 'Interrupt 0.4s']
color_list = ['blue', 'orange', 'green', 'red', 'cyan']
marker_list = ['o', '^', 's', '+', 'x']
ls_list = [':', '-.', '--', '-']

data_list = read_csv_data_to_list(csv_list, re_list)

#for dlist in data_list:

    # print 'length of list:' + str(len(slist))
    #print label[dlist[1]] + ' length of movement:' + str(length_of_movement(dlist[0], 10))

    #if movement == 3:
        #print 'avg ang of movement:' + str(avg_ang_of_straight(dlist[0]))


#if movement == 1:
    #plt_distance_from_ref_circle(data_list)

plot_raw_data(data_list)