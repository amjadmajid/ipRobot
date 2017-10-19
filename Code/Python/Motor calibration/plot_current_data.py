import csv
import math as m
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd


def chop_ends(list, thres):
    list = map(float, list)
    # Remove "zero" data
    i = 0
    while(list[i] < thres):
        i+=1
    #print 'i:' + str(i)
    list = list[i-10:]
    j = len(list)-1
    while(list[j] < thres):
        j-=1
    #print 'j:' + str(j)
    list = list[:j+10]
    return list


def csv_dict_reader(file_obj):
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

    x = np.linspace(0, 0.0002*len(maxc), len(maxc))

    plt.plot(x, maxc)
    plt.plot(x, avgc)
    #return avg_cur


# with open("min_freq.csv") as f_obj:
#     csv_dict_reader(f_obj)
#
# with open("half_freq_185.csv") as f_obj:
#     csv_dict_reader(f_obj)
#
# with open("max_freq_370.csv") as f_obj:
#     max_avg_cur = csv_dict_reader(f_obj)

dir = 'data_19-10/'

with open(dir + "r1_max_freq_100.csv") as f_obj:
    max_avg_cur = csv_dict_reader(f_obj)

with open(dir + "r1_max_freq_75.csv") as f_obj:
    max_avg_cur = csv_dict_reader(f_obj)

# with open(dir + "r1_max_freq_50.csv") as f_obj:
#     max_avg_cur = csv_dict_reader(f_obj)
#
# with open(dir + "r1_max_freq_25.csv") as f_obj:
#     max_avg_cur = csv_dict_reader(f_obj)
#
# with open(dir + "r1_max_freq_1.csv") as f_obj:
#     max_avg_cur = csv_dict_reader(f_obj)

with open(dir + "r1_max_freq_19step_ramp_to_76.csv") as f_obj:
    max_avg_cur = csv_dict_reader(f_obj)

with open(dir + "r1_max_freq_20step_ramp_to_100.csv") as f_obj:
    max_avg_cur = csv_dict_reader(f_obj)




plt.ylabel('Current (mA)')
plt.xlabel('Time (s)')
plt.xlim(xmin=0)  # this line
plt.ylim(ymin=0)  # this line
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
#