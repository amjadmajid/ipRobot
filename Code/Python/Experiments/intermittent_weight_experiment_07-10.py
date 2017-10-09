from __future__ import division
import matplotlib.pyplot as plt

set_dist = [50, 100, 150, 200]

int_1100_bat_dist = [[49.5, 49.6, 50.2], [98.4, 98.9, 99.5], [148.3, 147.6, 148.1], [191.4, 194.2, 194.8]]

dist_10g = [[41.1, 41.4, 40.9], [80.9, 80.5, 79.7], [125.2, 125.8, 123.3], [172.8, 166.6, 167.9]]

dist_20g = [[33.8, 35.7, 34.6], [68.4, 70.3, 68.3], [101.5, 104.1, 102.1], [142.3, 145.4, 140.8]]

dist_30g = [[29.9, 29.6, 30.1], [51.7, 55.3, 57.6], [84.4, 90.6, 85.2], [124.9, 123.6, 125.4]]


def list_avg(llist):
    i = 0
    avg = []
    while i < len(llist):
        avg.append(sum(llist[i])/len(llist[i]))
        i+=1
    return avg

plt.figure(1)
plt.plot(set_dist, list_avg(int_1100_bat_dist))
plt.plot(set_dist, list_avg(dist_10g))
plt.plot(set_dist, list_avg(dist_20g))
plt.plot(set_dist, list_avg(dist_30g))
plt.xlim(xmin=0)  # this line
plt.ylim(ymin=0)  # this line
plt.grid()
plt.xlabel('set distance (cm)')
plt.ylabel('measured distance (cm)')
plt.legend(('No weight added', '10 gram added', '20 gram added', '30 gram added'))

plt.show()