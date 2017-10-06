import numpy as np
import matplotlib.pyplot as plt


# Constant power (motor disabled at finish)
Hz120_20 = [24,21,16,22,19,22,17,22,24,15,22]
Hz120_40 = [38,41,42,43,40,42,41,40,40,41,40]
Hz120_60 = [57,60,60,60,61,55,60,60,60,60,60]
Hz120_80 = [82,83,79,80,80,80,80,80,80,80,80]

Hz140_20 = [24,19,25,25,19,24,19,25,23,18,19]
Hz140_40 = [39,44,45,38,43,35,49,35,38,42,34]
Hz140_60 = [64,65,61,61,61,63,63,57,60,52,61]
Hz140_80 = [79,82,77,83,77,76,84,83,80,75,72]

Hz140_160 = [165,162,158,162,157,157,163,160,160,157,163]

Hz100_20 = [23, 22, 20, 21, 18, 20, 20, 21, 21, 14, 20, 23]


#print 'mean bat_bot: ' + str(np.mean(bat_bot))

#print 'mean ip_bot: ' + str(np.mean(ip_bot))

#print 'stddev bat_bot: ' + str(np.std(bat_bot))

#print 'stddev ip_bot: ' + str(np.std(ip_bot))

# basic plot
plt.figure(1)
plt.boxplot([Hz120_20, Hz120_40, Hz120_60, Hz120_80])
plt.xticks([1, 2, 3, 4], ['20', '40', '60', '80'])

plt.figure(2)
plt.boxplot([Hz140_20, Hz140_40, Hz140_60, Hz140_80] )
plt.xticks([1, 2, 3, 4], ['20', '40', '60', '80'])

plt.figure(3)
plt.boxplot([Hz100_20, Hz120_20, Hz140_20])
plt.xticks([1, 2, 3], ['100', '120', '140'])

#plt.hist(bat_bot,normed=True)
#plt.hist(ip_bot,normed=True)

plt.show()
