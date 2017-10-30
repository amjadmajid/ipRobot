from __future__ import division
import math as m
import numpy as np
#import scipy as sp
from scipy.optimize import linprog
import matplotlib.pyplot as plt

##

As = 30 * m.pow(10, -3) * 30 * m.pow(10, -3)
Vc = 20 * m.pow(10, -3) * 15 * m.pow(10, -3) #* 2.3 * m.pow(10, -3)
Ap = 30 * m.pow(10, -3) * 30 * m.pow(10, -3)

As_or = 30 * m.pow(10, -3) * 30 * m.pow(10, -3)
I_const = 2 * m.pow(10, -3) / As_or
C_const = 22 * m.pow(10, -3) / Vc

Ms_const = 3.06 * m.pow(10, -3) / As
Mc_const = 1.88 * m.pow(10, -3) / Vc
Mp_const = 3.16 * m.pow(10, -3) / Ap
M_parts = (10.23 - 1.88 - 3.16) * m.pow(10, -3)

Eff = 0.01

def energy(t):
    return (1/2) * (1 / (C_const * Vc)) * m.pow(I_const * As, 2) * m.pow(t, 2)


def speed(t):
    return m.sqrt((((m.pow((I_const * As), 2) * m.pow(t, 2)) / (C_const * Vc)) * (Eff / 100))
                  / ((Ms_const * As) + (Mc_const * Vc) + (Mp_const * Ap) + M_parts))

t = np.linspace(0.0, 50.0, num=100)
y1 = np.array([])
y2 = np.array([])

for x in range(0, t.size):
    r1 = energy(x)
    r2 = speed(x)
    y1 = np.append(y1, r1)
    y2 = np.append(y2, r2)

print y2[90]

plt.subplot(2, 1, 1)
plt.plot(t, y1)
#plt.title('A tale of 2 subplots')
plt.ylabel('Energy (J)')

plt.subplot(2, 1, 2)
plt.plot(t, y2)
plt.xlabel('Charge Time (s)')
plt.ylabel('Speed (m/s)')

plt.show()

