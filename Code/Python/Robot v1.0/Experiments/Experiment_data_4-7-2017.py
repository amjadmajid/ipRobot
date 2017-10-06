import numpy as np
import matplotlib.pyplot as plt

# Move 50 cm, step frequency = 20Hz

ip_20_before = [40, 42, 49, 39.5, 43, 48.5, 47, 40, 48, 35]
ip_20_after = [43, 45.5, 48.5, 41, 48, 47, 38, 35, 48, 48.5]

print 'mean before: '+ str(np.mean(ip_20_before))
print 'mean after: ' + str(np.mean(ip_20_after))

plt.figure(1)
plt.ylabel('Distance in cm')
plt.ylim(0, 50)
plt.boxplot([ip_20_before, ip_20_after])
plt.xticks([1, 2], ['before', 'after'])

plt.show()