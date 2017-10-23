import csv
import math as m
import matplotlib.pyplot as plt
import numpy as np

dir1 = "data_23-10/"


def read_list_csv(name):
    with open(name, 'rb') as csvfile:
        data = csv.reader(csvfile)
        for x in data:
            dlist = map(int, x)
        return dlist


data1 = read_list_csv(dir1 + "r1_gyro_no_ramp_100_trg.csv")
data2 = read_list_csv(dir1 + "r1_gyro_20step_100_trg.csv")

x = np.linspace(0, 2, 200)

plt.figure(1)
plt.plot(x, data1)
plt.plot(x, data2)
plt.legend(('No ramp','20 step ramp'))
plt.xlabel('Time (s)')
plt.ylabel('Angular velocity (deg/s)')
plt.show()