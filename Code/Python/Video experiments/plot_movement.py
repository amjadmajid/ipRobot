import csv
import os
import re
import glob
import matplotlib.pyplot as plt


def read_movement_data(dfile):
    dlist = []
    with open(dfile) as csvfile:
        reader = csv.DictReader(csvfile)
        for row in reader:
            dlist.append((row['x'], row['y']))
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

ddir = 'Csv_Data/'
mdir = 'Video_Circle/'

csv_list = os.listdir(ddir+mdir)
print csv_list

pwm = 70
file_name = 'Video_trg_' + str(pwm) + '_r1_right_r30_battery'
re_list = ['', '_int1000', '_int1250', '_int500', '_int750']

lcnt = 0
re_cnt = 0
while lcnt < len(csv_list):
    if re.match(file_name + re_list[re_cnt] + '_\d', csv_list[lcnt], flags=0) is not None:
        #print file_name + re_list[re_cnt] + '_\d'

        slist = read_movement_data(ddir + mdir + csv_list[lcnt])

        plt.figure(re_cnt)
        plt.plot(*zip(*slist))
        plt.xlim(xmax=80)
        plt.ylim(ymax=80)
        plt.title(file_name + re_list[re_cnt])
        plt.xlabel('x distance in cm')
        plt.ylabel('y distance in cm')
        plt.axes().set_aspect('equal', 'datalim')
    else:
        if re_cnt < (len(re_list)-1):
            re_cnt += 1
            lcnt -= 1
        else:
            re_cnt = 0

    lcnt += 1
plt.show()
