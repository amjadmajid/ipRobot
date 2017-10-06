from __future__ import division
import matplotlib.pyplot as plt



time = 2 #sec
weight = [20.6, 31.0, 42.2, 53.3, 64.8, 71.2]
dist = [[57.3, 57.1, 56.8],[46.6, 47.6, 46.6], [42.1, 39.8, 39.3], [33.5, 32.0, 31.1], [22.5, 21.6, 20.5], [17.3, 17.5, 19.3]]

i = 0
avg = []
while i < len(dist):
    avg.append(sum(dist[i])/len(dist[i]))
    i+=1

avg_speed = [x / time for x in avg]
print avg_speed

plt.figure(1)
plt.plot(weight, avg)
plt.xlabel('weight (g)')
plt.ylabel('distance (cm)')

plt.figure(2)
plt.plot(weight, avg_speed)
plt.xlabel('weight (g)')
plt.ylabel('speed (cm/s)')

plt.show()