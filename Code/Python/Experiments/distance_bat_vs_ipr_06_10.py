from __future__ import division
import matplotlib.pyplot as plt

set_dist = [50, 100, 150, 200]

bat_dist = [[50.9, 51.1, 50.3], [102.3, 102.6, 104.2], [156.1, 156.7, 157.1], [213.2, 214.8, 215.0]]

int_1100_bat_dist = [[49.5, 49.6, 50.2], [98.4, 98.9, 99.5], [148.3, 147.6, 148.1], [191.4, 194.2, 194.8]]

int_825_bat_dist = [[45.1, 45, 45.1], [91.2, 93.4, 91.4], [141.1, 137.3, 136.2], [183.6, 183.4, 185.1]]

int_550_bat_dist = [[41.6, 40.1, 41.5], [85, 85.1, 86.8], [127.3, 125.8, 128.1], [175.2, 175.6, 172.9]]


def list_avg(llist):
    i = 0
    avg = []
    while i < len(llist):
        avg.append(sum(llist[i])/len(llist[i]))
        i+=1
    return avg

plt.figure(1)
plt.plot(set_dist, list_avg(bat_dist))
plt.plot(set_dist, list_avg(int_1100_bat_dist))
plt.plot(set_dist, list_avg(int_825_bat_dist))
plt.plot(set_dist, list_avg(int_550_bat_dist))
plt.xlim(xmin=0)  # this line
plt.ylim(ymin=0)  # this line
plt.grid()
plt.xlabel('set distance (cm)')
plt.ylabel('measured distance (cm)')
plt.legend(('Battery powered', 'BP with a software interrupt every 1.1 sec', 'BP with a software interrupt every 0.825 sec', 'BP with a software interrupt every 0.55 sec'))

plt.show()