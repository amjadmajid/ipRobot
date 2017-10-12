from __future__ import division
import matplotlib.pyplot as plt

# set_dist = [50, 100, 150, 200]
#
# t20_dist = [48.03, 98.97, 150.3, 197.6]
# t30_dist = [47.4, 98.3, 149.3, 198.23]
# t40_dist = [46.9, 98.23, 148.47, 199.63]
# t50_dist = [46.53, 96.8, 146.23, 199.87]
#
# plt.figure(1)
# plt.plot(set_dist, t20_dist)
# plt.plot(set_dist, t30_dist)
# plt.plot(set_dist, t40_dist)
# plt.plot(set_dist, t50_dist)
# plt.xlim(xmin=0)  # this line
# plt.ylim(ymin=0)  # this line
# plt.grid()
# #plt.title('China 17% Monocrystaline 50x50')
# plt.xlabel('set distance (cm)')
# plt.ylabel('measured distance (cm)')
# plt.legend(('18.2 cm/s','21.6 cm/s', '24.3 cm/s', '26.5 cm/s'))

target_duty = [20, 30, 40, 50, 60, 70]

speed = [18.2, 21.6, 24.3, 26.5, 28.5, 29.4]

plt.figure(1)
plt.plot(target_duty, speed)
plt.xlim(xmin=0)  # this line
plt.ylim(ymin=0)  # this line
plt.grid()
#plt.title('China 17% Monocrystaline 50x50')
plt.xlabel('target duty-cycle')
plt.ylabel('speed (cm/s)')
#plt.legend(('18.2 cm/s','21.6 cm/s', '24.3 cm/s', '26.5 cm/s'))

plt.show()