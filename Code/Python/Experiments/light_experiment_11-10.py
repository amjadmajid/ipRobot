from __future__ import division
import matplotlib.pyplot as plt

set_dist = [10, 30, 50]

cp_60w = [[3.034, 3.093, 3.124], [6.489, 6.389, 6.481], [26.04, 26.71, 26.08]]

cp_120w = [[1.156, 2.716, 2.706], [6.703, 6.766, 6.694], [17.09, 18.051, 17.64]]

cp_150w_w = [[1.002, 0.9503, 1.029], [7.846, 7.795, 7.843], [17.72, 17.87, 18.15]]

cp_150w_r = [[0.7548, 0.7074, 0.74], [1.79, 1.803, 2.048], [9.276, 9.615, 9.4696]]

ns_60w = [[2.836, 2.749, 2.846], [11.25, 11.6, 11.73], [52.07, 53.84, 54.09]]

ns_120w = [[1.616, 1.615, 1.6], [4.055, 4.049, 4.2265], [11.82, 12.21, 12.24]]

ns_150w_w = [[0.7703, 0.7737, 0.7738], [9.602, 10.07, 9.958], [22.36, 22.2, 24.74]]

ns_150w_r = [[1.605, 1.858, 1.852], [10.1, 10.3, 10.09], [46.53, 47.63, 46.71]]

def list_avg(llist):
    i = 0
    avg = []
    while i < len(llist):
        avg.append(sum(llist[i])/len(llist[i]))
        i+=1
    return avg

plt.figure(1)
plt.plot(set_dist, list_avg(cp_60w), '-o')
plt.plot(set_dist, list_avg(cp_120w), '-o')
plt.plot(set_dist, list_avg(cp_150w_w), '-o')
plt.plot(set_dist, list_avg(cp_150w_r), '-o')
plt.xlim(xmin=0)  # this line
plt.ylim(ymin=0)  # this line
plt.grid()
plt.title('China 17% Monocrystaline 50x50')
plt.xlabel('distance from solar panel (cm)')
plt.ylabel('charge time (s)')
plt.legend(('60w halogen','120w halogen', 'white 150w ir lamp', 'red 150w ir lamp'))

plt.figure(2)
plt.plot(set_dist, list_avg(ns_60w), '-o')
plt.plot(set_dist, list_avg(ns_120w), '-o')
plt.plot(set_dist, list_avg(ns_150w_w), '-o')
plt.plot(set_dist, list_avg(ns_150w_r), '-o')
plt.xlim(xmin=0)  # this line
plt.ylim(ymin=0)  # this line
plt.grid()
plt.title('Azurspace 28% Triple Junction GaAs 3G28C 80x40 (Nuna)')
plt.xlabel('distance from solar panel (cm)')
plt.ylabel('charge time (s)')
plt.legend(('60w halogen','120w halogen', 'white 150w ir lamp', 'red 150w ir lamp'))

plt.show()