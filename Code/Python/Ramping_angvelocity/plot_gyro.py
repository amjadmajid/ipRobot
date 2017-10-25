import csv
import math as m
import matplotlib.pyplot as plt
import numpy as np

dir1 = "data_smax_200/"


def running_mean(x, N):
    cumsum = np.cumsum(np.insert(x, 0, 0))
    return (cumsum[N:] - cumsum[:-N]) / N


def read_list_csv(name):
    with open(name, 'rb') as csvfile:
        data = csv.reader(csvfile)
        for x in data:
            dlist = map(int, x)
        return dlist


data1 = read_list_csv(dir1 + "r1_gyro_no_ramp_100_trg_v2.csv")
data2 = read_list_csv(dir1 + "r1_gyro_10step_100_trg.csv")
data3 = read_list_csv(dir1 + "r1_gyro_20step_100_trg_v2.csv")

data1 = running_mean(map(float, data1), 10)
data2 = running_mean(map(float, data2), 10)
data3 = running_mean(map(float, data3), 10)

data4 = read_list_csv(dir1 + "r1_gyro_no_ramp_80_trg.csv")
data5 = read_list_csv(dir1 + "r1_gyro_10step_80_trg.csv")
data6 = read_list_csv(dir1 + "r1_gyro_20step_80_trg.csv")

data4 = running_mean(map(float, data4), 10)
data5 = running_mean(map(float, data5), 10)
data6 = running_mean(map(float, data6), 10)

x = np.linspace(0, 191*0.01, 191)

fig1 = plt.figure(1)
ax1 = fig1.add_subplot(211)
ax1.plot(x, data1)
ax1.plot(x, data2)
ax1.plot(x, data3)
ax1.set_title('Robot1, PWM target 100')
ax1.set_ylabel('Angular velocity (deg/s)')
ax1.legend(('No ramp','10 step ramp','20 step ramp'))
ax2 = fig1.add_subplot(212)
ax2.set_title('Robot1, PWM target 80')
ax2.set_ylabel('Angular velocity (deg/s)')
ax2.legend(('No ramp','10 step ramp','20 step ramp'))
ax2.plot(x, data4)
ax2.plot(x, data5)
ax2.plot(x, data6)
plt.xlabel('Time (s)')

data1 = read_list_csv(dir1 + "r2_gyro_no_ramp_100_trg_v2.csv")
data2 = read_list_csv(dir1 + "r2_gyro_10step_100_trg.csv")
data3 = read_list_csv(dir1 + "r2_gyro_20step_100_trg_v2.csv")

data1 = running_mean(map(float, data1), 10)
data2 = running_mean(map(float, data2), 10)
data3 = running_mean(map(float, data3), 10)

data4 = read_list_csv(dir1 + "r2_gyro_no_ramp_80_trg.csv")
data5 = read_list_csv(dir1 + "r2_gyro_10step_80_trg.csv")
data6 = read_list_csv(dir1 + "r2_gyro_20step_80_trg.csv")

data4 = running_mean(map(float, data4), 10)
data5 = running_mean(map(float, data5), 10)
data6 = running_mean(map(float, data6), 10)

#x = np.linspace(0, 200*0.01, 200)

fig2 = plt.figure(2)
ax1 = fig2.add_subplot(211)
ax1.plot(x, data1)
ax1.plot(x, data2)
ax1.plot(x, data3)
ax1.set_title('Robot2, PWM target 100')
ax1.set_ylabel('Angular velocity (deg/s)')
ax1.legend(('No ramp','10 step ramp','20 step ramp'))
ax2 = fig2.add_subplot(212)
ax2.set_title('Robot2, PWM target 80')
ax2.set_ylabel('Angular velocity (deg/s)')
ax2.legend(('No ramp','10 step ramp','20 step ramp'))
ax2.plot(x, data4)
ax2.plot(x, data5)
ax2.plot(x, data6)
plt.xlabel('Time (s)')
plt.show()