import csv
import math as m
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd


def chop_ends(dlist, thres):
    dlist = map(float, dlist)
    # Remove "zero" data
    i = 0
    while dlist[i] < thres:
        i+=1
    #print 'i:' + str(i)
    dlist = dlist[i-10:]
    j = len(dlist)-1
    while dlist[j] < thres:
        j-=1
    #print 'j:' + str(j)
    dlist = dlist[:j+10]
    return dlist


def get_max_avg_cur(file_obj):
    """
    Read a CSV file using csv.DictReader
    """
    reader = csv.DictReader(file_obj, delimiter=',')
    time = []
    avg_cur = []
    avg_vol = []
    max_cur = []
    for line in reader:
         time.append(line["Time (s)"])
         avg_cur.append(line["Main Avg Current (mA)"])
         avg_vol.append(line["Main Avg Voltage (V)"])
         max_cur.append(line["Main Max Current (mA)"])

    avg_cur = chop_ends(avg_cur, 2)

    ws = 100
    maxc = pd.Series(avg_cur).rolling(ws).max().dropna().tolist()
    avgc = pd.Series(avg_cur).rolling(ws).mean().dropna().tolist()

    print 'max_max: ' + str(max(maxc))
    print 'avg_max: ' + str(max(avgc))
    x = np.linspace(0, 0.0002 * len(maxc), len(maxc))
    ax1.plot(x, maxc)
    ax1.axhline(y=110,linewidth=1, color = 'r')
    ax2.plot(x, avgc)


dir = 'data_19-10/'

fig1 = plt.figure(1)
fig1.suptitle('PWM target 140', fontsize=12, fontweight='normal')
ax1 = fig1.add_subplot(211)
ax1.set_title('Max current')
ax1.set_ylabel('Current (mA)')
ax2 = fig1.add_subplot(212)
ax2.set_title('Average current')
ax2.set_ylabel('Current (mA)')

with open(dir + "r1_max_freq_140.csv") as f_obj:
    get_max_avg_cur(f_obj)

with open(dir + "r1_max_freq_10step_ramp_to_140.csv") as f_obj:
    get_max_avg_cur(f_obj)

with open(dir + "r1_max_freq_20step_ramp_to_140.csv") as f_obj:
    get_max_avg_cur(f_obj)

fig2 = plt.figure(2)
fig2.suptitle('PWM target 120', fontsize=12, fontweight='normal')
ax1 = fig2.add_subplot(211)
ax1.set_title('Max current')
ax1.set_ylabel('Current (mA)')
ax2 = fig2.add_subplot(212)
ax2.set_title('Average current')
ax2.set_ylabel('Current (mA)')

with open(dir + "r1_max_freq_120.csv") as f_obj:
    get_max_avg_cur(f_obj)

with open(dir + "r1_max_freq_10step_ramp_to_120.csv") as f_obj:
    get_max_avg_cur(f_obj)

with open(dir + "r1_max_freq_20step_ramp_to_120.csv") as f_obj:
    get_max_avg_cur(f_obj)


fig3 = plt.figure(3)
fig3.suptitle('PWM target 100', fontsize=12, fontweight='normal')
ax1 = fig3.add_subplot(211)
ax1.set_title('Max current')
ax1.set_ylabel('Current (mA)')
ax2 = fig3.add_subplot(212)
ax2.set_title('Average current')
ax2.set_ylabel('Current (mA)')

with open(dir + "r1_max_freq_100.csv") as f_obj:
    get_max_avg_cur(f_obj)

with open(dir + "r1_max_freq_10step_ramp_to_100.csv") as f_obj:
    get_max_avg_cur(f_obj)

with open(dir + "r1_max_freq_20step_ramp_to_100.csv") as f_obj:
    get_max_avg_cur(f_obj)

plt.show()


# V = 0.195
# arr = []
# for x in max_avg_cur:
#     if x > 110:
#         arr.append(x)
#
# I = 0
# for y in arr:
#     I += (y - 119)
# I = I / (len(arr))
#
# print "Ecap = " + str(V*(I*m.pow(10,-3))*0.011)
#
# print "Ecap2 = " + str(0.5*220*m.pow(10,-6)*m.pow(V,2))
#
# print 'time (ms): ' + str(0.2*len(arr))
