import numpy as np
import matplotlib.pyplot as plt

# pwm at different duty cyles (avg because of gain between left and right motor)

pwm_150 = [25.5, 25, 25.5, 25.5]
pwm_200 = [34.5, 34, 34.5, 34.5]
pwm_250 = [38, 37.5, 38, 38]
pwm_300 = [40.5, 41, 40.5, 40.5]
pwm_350 = [41.5, 41.5, 41, 41.5]
pwm_400 = [42, 42, 42, 42.5]


x = [0.5*150*(1+1.4), 0.5*200*(1+1.35), 0.5*250*(1+1.35), 0.5*300*(1+1.3), 0.5*350*(1+1.3), 0.5*400*(1+1.3)]

y = [np.mean(pwm_150), np.mean(pwm_200), np.mean(pwm_250), np.mean(pwm_300), np.mean(pwm_350), np.mean(pwm_400)]

f, (ax1, ax2) = plt.subplots(2, 1, sharey=True)
ax1.set_ylabel('distance with single charge (cm)')
ax1.plot([i/10 for i in x],y, 'ro')

I_150 = [35.5, 34.7, 35]
I_200 = [40.3, 39.6, 39.4]
I_250 = [43.9, 42.4, 42.5]
I_300 = [43.5, 43.3, 43]
I_350 = [46.8, 47.2, 46.9]
I_400 = [48, 47, 47.5]

y = [np.mean(I_150), np.mean(I_200), np.mean(I_250), np.mean(I_300), np.mean(I_350), np.mean(I_400)]
ax2.set_xlabel('pwm duty cycle (%)')
ax2.set_ylabel('avg current (mA)')
ax2.plot([i/10 for i in x],y, 'b-')
plt.show()
