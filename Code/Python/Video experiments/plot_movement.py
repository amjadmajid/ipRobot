import csv
import os
import re
import glob
import math as m
from collections import OrderedDict
import matplotlib.pyplot as plt


def read_movement_data(dfile):
    dlist = []
    with open(dfile) as csvfile:
        reader = csv.DictReader(csvfile)
        for row in reader:
            dlist.append((float(row['x']), float(row['y'])))
    return dlist


def write_movement_data(fname, dlist):
    with open(fname, 'wb') as out:
        csv_out = csv.writer(out)
        csv_out.writerow(['x', 'y'])
        for row in dlist:
            csv_out.writerow(row)


def deduplicate(slist):
    tcnt = 0
    while tcnt < len(slist)-1:
        if slist[tcnt] == slist[tcnt+1]:
            slist.pop(tcnt)
        else:
            tcnt += 1
    return slist


def length_of_movement(dlist):
    jump = int(round(len(dlist)/10))
    cnt = 0
    length = 0
    while cnt < len(dlist)-jump:
        length += m.sqrt((dlist[cnt+jump][0] - dlist[cnt][0]) ** 2 + (dlist[cnt+jump][1] - dlist[cnt][1]) ** 2)
        cnt += jump
    return length


ddir = 'Csv_Data/'

movement = 1
set = 2
pwm = ['30', '50', '70']
if movement == 1:
    mdir = 'Video_Circle/'
    file_name = 'Video_trg_' + pwm[set] + '_r1_right_r30_battery'
elif movement == 2:
    mdir = 'Video_Square/'
    file_name = 'Video_trg_70_r1_right_battery'
else:
    mdir = 'Video_Straight/'
    file_name = 'Video_trg_30_r1_75_battery'

csv_list = os.listdir(ddir+mdir)
print csv_list

re_list = ['', '_int1000', '_int1250', '_int500', '_int750']

label = ['No Interrupt', 'Interrupt 1.25s', 'Interrupt 1.0s', 'Interrupt 0.75s', 'Interrupt 0.5s']
color_list = ['blue', 'orange', 'green', 'red', 'yellow']
marker_list = ['o', '^', 's', '+', 'x']
ls_list = [':', '-.', '--', '-']

lcnt = 0
re_cnt = 0
while lcnt < len(csv_list):
    if re.match(file_name + re_list[re_cnt] + '_\d', csv_list[lcnt], flags=0) is not None:
        print file_name + re_list[re_cnt]

        slist = read_movement_data(ddir + mdir + csv_list[lcnt])
        print 'length of list:' + str(len(slist))
        print 'length of movement:' + str(length_of_movement(slist))

        plt.plot(*zip(*slist), label=label[re_cnt], color=color_list[re_cnt], marker=marker_list[re_cnt], markevery=20)

    else:
        if re_cnt < (len(re_list) - 1):
            re_cnt += 1
            lcnt -= 1
        else:
            re_cnt = 0

    lcnt += 1

if movement == 1:
    circ = plt.Circle((35, 38), radius=30, color='black', fill=False)
    plt.gca().add_patch(circ)
plt.xlim(xmin=-10, xmax=90)
plt.ylim(ymin=-10, ymax=90)
plt.title(file_name)
plt.xlabel('x distance in cm')
plt.ylabel('y distance in cm')
plt.axes().set_aspect('equal', 'datalim')
# remove duplicate labels
handles, labels = plt.gca().get_legend_handles_labels()
by_label = OrderedDict(zip(labels, handles))
plt.legend(by_label.values(), by_label.keys(), loc='upper right')
plt.show()
