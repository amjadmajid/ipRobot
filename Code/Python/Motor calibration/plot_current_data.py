import csv
import math as m
import matplotlib.pyplot as plt
from scipy.signal import savgol_filter

lines = []

# with open('min_freq.csv') as f:
#     for line in f.readlines():
#         lines.append(line)
#
# print lines

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

    avg_cur = savgol_filter(avg_cur, 101, 2)  # window size 51, polynomial order 3

    print 'max = ' + str(max(avg_cur))
    print 'max2 = ' + str(max(max_cur))

    plt.figure(1)
    plt.plot(avg_cur)
    #plt.plot(max_cur)

    return avg_cur


with open("min_freq.csv") as f_obj:
    csv_dict_reader(f_obj)

with open("half_freq_185.csv") as f_obj:
    csv_dict_reader(f_obj)

with open("max_freq_370.csv") as f_obj:
    max_avg_cur = csv_dict_reader(f_obj)

V = 0.195
arr = []
for x in max_avg_cur:
    if x > 110:
        arr.append(x)

I = 0
for y in arr:
    I += (y - 119)
I = I / (len(arr))

print "Ecap = " + str(V*(I*m.pow(10,-3))*0.011)

print "Ecap2 = " + str(0.5*220*m.pow(10,-6)*m.pow(V,2))

print 'time (ms): ' + str(0.2*len(arr))

plt.show()