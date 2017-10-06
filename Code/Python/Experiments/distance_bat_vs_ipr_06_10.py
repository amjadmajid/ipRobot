from __future__ import division
import matplotlib.pyplot as plt

set_dist = [50, 100, 150, 200]

bat_dist = [[50.9, 51.1, 50.3], [102.3, 102.6, 104.2], [156.1, 156.7, 157.1], [213.2, 214.8, 215.0]]

int_bat_dist = [[49.5, 49.6, 50.2], [98.4, 98.9, 99.5], [148.3, 147.6, 148.1], [191.4, 194.2, 194.8]]

def list_avg(llist):
    i = 0
    avg = []
    while i < len(llist):
        avg.append(sum(llist[i])/len(llist[i]))
        i+=1
    return avg

plt.figure(1)
plt.plot(set_dist, list_avg(bat_dist))
plt.plot(set_dist, list_avg(int_bat_dist))
plt.xlim(xmin=0)  # this line
plt.ylim(ymin=0)  # this line
plt.grid()
plt.xlabel('set distance (cm)')
plt.ylabel('measured distance (cm)')
plt.legend(('Battery powered', 'BP with a software interrupt every 1.1 sec'))

plt.show()