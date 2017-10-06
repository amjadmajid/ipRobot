import numpy as np
import matplotlib.pyplot as plt

# pwm at different duty cyles (avg because of gain between left and right motor)

# 200L
pwm_225 = [32.5, 32, 33.5]

pwm_281 = [36, 35, 37]
# 300L
pwm_338 = [39, 38.5, 39]

pwm_394 = [39.5, 39.5, 39]
# 400L
pwm_450 = [41.5, 41.0, 41.5]

pwm_495 = [41, 41, 41.5]
# 500L
pwm_550 = [44, 44.5, 44]

x = [225, 281, 338, 394, 450, 495,  550]

y = [np.mean(pwm_225), np.mean(pwm_281), np.mean(pwm_338), np.mean(pwm_394), np.mean(pwm_450), np.mean(pwm_495), np.mean(pwm_550)]

plt.figure(1)
# plt.xticks([0, 1, 2, 3, 4, 5, 6], ['225', '281', '338', '394', '450', '495',  '550'])

plt.plot(x,y, 'ro')
plt.plot([225, 550], [33, 44], 'k-')
plt.show()
