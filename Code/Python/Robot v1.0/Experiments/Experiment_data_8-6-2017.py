import numpy as np
import matplotlib.pyplot as plt

bat_bot = [51.6, 50.8, 51.2, 50.8, 51.5, 51.3, 51.3, 51.3, 51.4, 51.3]

ip_bot = [36.3, 37.0, 37.4, 39.1, 38.6, 40.3, 40.0, 43.1, 45.1, 46.4]

print 'mean bat_bot: ' + str(np.mean(bat_bot))

print 'mean ip_bot: ' + str(np.mean(ip_bot))

print 'stddev bat_bot: ' + str(np.std(bat_bot))

print 'stddev ip_bot: ' + str(np.std(ip_bot))

# basic plot
plt.figure(figsize=(5, 5))
plt.ylabel('Distance (cm)')
plt.boxplot([bat_bot, ip_bot])
plt.ylim(ymin=0)
plt.xticks([1,2], ['Battery-powered robot', 'Transiently-powered robot'])

#plt.figure(2)
#plt.hist(bat_bot,normed=True)
#plt.hist(ip_bot,normed=True)

plt.show()
