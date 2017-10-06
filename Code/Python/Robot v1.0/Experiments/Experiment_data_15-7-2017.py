import numpy as np
import matplotlib.pyplot as plt

# speed vs distance to power interrupt

p20 = [14, 16, 14, 16]
p30 = [16, 17.5, 17.5, 17.5]
p40 = [18, 16, 18, 18]
p50 = [19, 18, 18.5, 19]
p60 = [18.5, 19, 19, 19]
p70 = [18.5, 18.5, 18.5, 19]
p80 = [18.5, 19.5, 20, 19]
p90 = [19, 19.5, 19, 19]
p100 = [25.5, 26, 26, 26.5]

y = [0, np.mean(p20), np.mean(p30),np.mean(p40),np.mean(p50),np.mean(p60),np.mean(p70),np.mean(p80),np.mean(p90), np.mean(p100)]

# use delay instead of timer and only disable drv1 no i2c required

d10 = [17.5, 18.5, 18.5, 18, 18]
d20 = [21, 21, 21, 21, 21.5]
d30 = [22.5, 22, 23, 22.5, 23]
d40 = [21.5, 23.5, 23, 22, 23]
d50 = [22.5, 23, 23, 23.5, 23]
d60 = [22.5, 22.5, 24, 23, 24]
d70 = [23, 22.5, 24, 23, 23.5]
d80 = [22, 22.5, 22, 23, 22.5]
d90 = [23.5, 22, 24, 23, 24.5]


y2 = [np.mean(d10), np.mean(d20), np.mean(d30),np.mean(d40),np.mean(d50),np.mean(d60),np.mean(d70),np.mean(d80),np.mean(d90), 0]

plt.figure(1)
x = np.linspace(10, 100, 10)
plt.xlabel('duty cycle (%)')
plt.ylabel('distance to interrupt (cm)')
plt.plot(x, y, 'bo')
plt.plot(x, y2, 'ro')
plt.show()