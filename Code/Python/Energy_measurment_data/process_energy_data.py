import csv
import math as m
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd


def chop_ends(dlist, thres, inv):
    if inv != 1:
        dlist = map(float, dlist)
        # Remove "zero" data
        i = 0
        while dlist[i] < thres:
            i+=1
        while dlist[i] > thres:
            i+=1
        #print 'i:' + str(i)
        dlist = dlist[i:]
        j = len(dlist)-1
        while dlist[j] > thres:
            j-=1
        #print 'j:' + str(j)
        dlist = dlist[:j]
    else:
        dlist = map(float, dlist)
        # Remove "zero" data
        i = 0
        while dlist[i] < thres:
            i+=1
        #print 'i:' + str(i)
        dlist = dlist[i:]
        j = len(dlist)-1
        while dlist[j] < thres:
            j-=1
        #print 'j:' + str(j)
        dlist = dlist[:j]
    return dlist


def get_max_avg_cur(file, inv):
    with open(file) as f_obj:
        """
        Read a CSV file using csv.DictReader
        """
        reader = csv.DictReader(f_obj, delimiter=',')
        time = []
        avg_cur = []
        avg_vol = []
        max_cur = []
        for line in reader:
             time.append(line["Time (s)"])
             avg_cur.append(line["Main Avg Current (mA)"])
             avg_vol.append(line["Main Avg Voltage (V)"])
             max_cur.append(line["Main Max Current (mA)"])

    avg_cur = chop_ends(avg_cur, 3, inv)

    ws = 100
    avgc = pd.Series(avg_cur).rolling(ws).mean().dropna().tolist()

    print 'avg_max: ' + str(max(avgc))
    avg5000 = sum(avgc[len(avgc)-5001:len(avgc)-1]) / 5000
    print 'avg last 5000 current values: ' + str(avg5000)
    x = np.linspace(0, 0.0002 * len(avgc), len(avgc))
    plt.plot(x, avgc)
    plt.xlabel("Time (s)")
    plt.ylabel("Current (mA)")

plt.figure(1)
get_max_avg_cur("Wisp_Enable_8Mhz_to_LED.csv", 0)
plt.figure(2)
get_max_avg_cur("R1_Wisp_Enable_8Mhz_to_LED.csv", 0)
plt.figure(3)
get_max_avg_cur("R1_Wisp_Enable_8Mhz_and_gyro_to_LED.csv", 0)
plt.figure(4)
get_max_avg_cur("R1_Wisp_Enable_8Mhz_and_proximity_to_LED.csv", 0)
plt.figure(5)
get_max_avg_cur("R1_Wisp_Enable_8Mhz_and_hbridge_to_LED.csv", 0)
plt.figure(6)
get_max_avg_cur("R1_Wisp_Enable_8Mhz_and_gyro_and_hbridge_to_LED.csv", 0)

plt.figure(7)
get_max_avg_cur("Wisp_Enable_8Mhz_to_Enable_green_LED(wisp5.0).csv", 1)
plt.figure(8)
get_max_avg_cur("Wisp_Enable_8Mhz_to_Enable_red_LED(wisp5.1).csv", 1)

plt.show()


