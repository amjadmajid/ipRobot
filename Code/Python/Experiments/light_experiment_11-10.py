from __future__ import division
import matplotlib.pyplot as plt

cp_60w = [[3.034, 3.093, 3.124], [6.489, 6.389, 6.481], [26.04, 26.71, 26.08]]

cp_120w = [[1.156, 2.716, 2.706], [6.703, 6.766, 6.694], [17.09, 18.051, 17.64]]

cp_150w_w = [[1.002, 0.9503, 1.029], [7.846, 7.795, 7.843], [17.72, 17.87, 18.15]]

cp_150w_r = [[0.7548, 0.7074, 0.74], [1.79, 1.803, 2.048], [9.276, 9.615, 9.4696]]

dk_60w = [2.7566666667, 8.759, 49.6966666667]

dk_120w = [0.8214, 6.214, 16.9966666667]

dk_150w_w = [0.5065, 2.92, 10.1533333333]

dk_150w_r = [0.5634666667, 1.632, 7.6316666667]

ns_60w = [[2.836, 2.749, 2.846], [11.25, 11.6, 11.73], [52.07, 53.84, 54.09]]

ns_120w = [[1.616, 1.615, 1.6], [4.055, 4.049, 4.2265], [11.82, 12.21, 12.24]]

ns_150w_w = [[0.7703, 0.7737, 0.7738], [9.602, 10.07, 9.958], [22.36, 22.2, 24.74]]

ns_150w_r = [[1.605, 1.858, 1.852], [10.1, 10.3, 10.09], [46.53, 47.63, 46.71]]


def list_avg(llist):
    i = 0
    avg = []
    while i < len(llist):
        avg.append(sum(llist[i])/len(llist[i]))
        i += 1
    return avg

cp = [list_avg(cp_60w), list_avg(cp_120w), list_avg(cp_150w_w), list_avg(cp_150w_r)]
dk = [dk_60w, dk_120w, dk_150w_w, dk_150w_r]
ns = [list_avg(ns_60w), list_avg(ns_120w), list_avg(ns_150w_w), list_avg(ns_150w_r)]


def plt_time(var, yl, ym, f):
    fsize = 12
    set_dist = ['10', '30', '50']
    ll = [':o', '-.o', '--o', '-o']
    cnt = 0
    plt.figure(figsize=(6,3))
    while cnt < 4:
        plt.plot([1, 2, 3], [k * f for k in var[cnt]], ll[cnt])
        #plt.bar([2+cnt, (2+cnt)+6, (2+cnt)+12], [k * f for k in var[cnt]], width=1)
        cnt += 1
    plt.ylim(ymin=0, ymax=ym)  # this line
    plt.xlabel('Distance from solar panel (cm)', fontsize=fsize)
    plt.ylabel(yl, fontsize=fsize)
    plt.xticks([1, 2, 3], set_dist, fontsize=fsize)
    plt.yticks(fontsize=fsize)
    plt.legend(('60W halogen', '120W halogen', '150W IR lamp white', '150W IR lamp red'), loc='upper left', fontsize=fsize)


frac = (80 * 40 - (13.5 * 13.5)) / 1200
# Ebay
plt_time(cp, 'Charge time (s)', 60, 1)
# IXYS
plt_time(dk, 'Charge time (s)', 60, 1)
# Azurspace
plt_time(ns, 'Charge time (s)', 60,1)

plt.show()

lux_60w = [5790, 2460, 577]
lux_120w = [8760, 2496, 1179]
lux_150w_w = [9860, 2046, 1027]
lux_150w_r = [10550, 2652, 821]

temp_60w = [25, 23, 22]
temp_120w = [35, 25, 23]
temp_150w_w = [37, 25, 26]
temp_150w_r = [66, 37, 31]

lux = [lux_60w, lux_120w, lux_150w_w, lux_150w_r]
temp = [temp_60w, temp_120w, temp_150w_w, temp_150w_r]

plt_time(lux, 'Illuminance (lux))', 11000, 1)

plt_time(temp, 'Temperature (C)', 70, 1)

plt.show()

