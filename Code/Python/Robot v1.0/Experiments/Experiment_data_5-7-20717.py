import numpy as np
import matplotlib.pyplot as plt

# Intermittently powered, 20Hz, 32 cm
# Count before, 12 checkpoints
arr_b_12 = [16.5, 13.5, 15.2, 16.4, 13.5]
# Count after, 12 checkpoints
arr_a_12 = [32.5, 27, 26.5, 27.5, 31]

# Count before, 22 checkpoints
arr_b_22 = [27.5, 28.5, 25, 22.5, 28]
# Count after, 22 checkpoints
arr_a_22 = [45.5, 43, 49.5, 35, 35]

print 'mean before 12cp: '+ str(np.mean(arr_b_12))
print 'mean after 12cp: ' + str(np.mean(arr_a_12))
print 'mean before 22cp: '+ str(np.mean(arr_b_22))
print 'mean after 22cp: ' + str(np.mean(arr_a_22))

plt.figure(1)
plt.ylabel('Distance in cm')
plt.ylim(0, 50)
plt.boxplot([arr_b_12, arr_a_12, arr_b_22, arr_a_22])
plt.xticks([1, 2, 3, 4], ['before 12cp', 'after 12cp', 'before 22cp', 'after 22cp'])

plt.show()