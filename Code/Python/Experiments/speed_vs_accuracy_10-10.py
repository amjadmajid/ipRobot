from __future__ import division
import matplotlib.pyplot as plt

set_dist = [50, 100, 150, 200]

bat_dist = [[50.9, 51.1, 50.3], [102.3, 102.6, 104.2], [156.1, 156.7, 157.1], [213.2, 214.8, 215.0]]

int_75_bat_dist = [[49.5, 49.6, 50.2], [98.4, 98.9, 99.5], [148.3, 147.6, 148.1], [191.4, 194.2, 194.8]]

int_56_bat_dist = [[45.5, 49.2, 46.1], [92.3, 91.8, 92.2], [137.3, 138.2, 137.1], [172.5, 175.5, 181.2]]

int_37_bat_dist = [[43.8, 44.2, 43.6], [88.7, 90.2, 89.6], [133.7, 134.5, 134.30], [177.6, 176.6, 176.9]]


def list_avg(llist):
    i = 0
    avg = []
    while i < len(llist):
        avg.append(sum(llist[i])/len(llist[i]))
        i+=1
    return avg

plt.figure(1)
plt.plot(set_dist, list_avg(bat_dist))
plt.plot(set_dist, list_avg(int_75_bat_dist))
plt.plot(set_dist, list_avg(int_56_bat_dist))
plt.plot(set_dist, list_avg(int_37_bat_dist))
plt.xlim(xmin=0)  # this line
plt.ylim(ymin=0)  # this line
plt.grid()
plt.xlabel('set distance (cm)')
plt.ylabel('measured distance (cm)')
plt.legend(('Bat no int (28 cm/s)','Set speed 28 (cm/s)', 'Set speed 26 (cm/s)', 'Set speed 22 (cm/s)'))

plt.show()