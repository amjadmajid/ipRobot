from __future__ import division
import math as m
import numpy as np
import scipy as sp
import matplotlib.pyplot as plt

##

plt.plot([40, 40], [0, 20], 'k-')      # 0
plt.plot([80, 120], [20, 20], 'k-')    # 1
plt.plot([20, 20], [20, 40], 'k-')     # 2
plt.plot([60, 60], [20, 40], 'k-')     # 3
plt.plot([20, 100], [40, 40], 'k-')    # 4
plt.plot([40, 40], [40, 60], 'k-')     # 5
plt.plot([80, 80], [40, 60], 'k-')     # 6
plt.plot([0, 20], [60, 60], 'k-')      # 7
plt.plot([60, 80], [60, 60], 'k-')     # 8
plt.plot([60, 60], [60, 100], 'k-')    # 9
plt.plot([100, 100], [60, 80], 'k-')   # 10
plt.plot([20, 60], [80, 80], 'k-')     # 11
plt.plot([80, 100], [80, 80], 'k-')    # 12
plt.plot([20, 20], [80, 100], 'k-')    # 13
plt.plot([80, 80], [80, 100], 'k-')    # 14
plt.plot([80, 120], [100, 100], 'k-')  # 15
plt.plot([40, 40], [100, 120], 'k-')   # 16

plt.ylim([0,120])
plt.xlim([0,120])





#plt.ylabel('some numbers')
plt.show()

##
