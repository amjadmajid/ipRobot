import numpy as np
import matplotlib.pyplot as plt

# set distance to approx 120cm

target = [123-5, 122.5-5, 123.5-5, 123.5-5]

cp6_b = [60-5, 60-5, 76-6, 75-5]
cp6_a = [135-5, 132-5, 142-5, 144.5-5]

cp7_b = [92.5-5, 95.5-5, 94-5, 95.5]
cp7_a = [165-5, 156.5-5, 164.5-5, 170-5]

cp8_b = [98.5-5, 99.5-5, 96.5-5, 101.5-5]
cp8_a = [181.5-5, 185.5-5, 191.5-5, 188.5-5]

cp9_b = [110-5, 109-5, 108-5, 113-5]
cp9_a = [200-5, 201-5, 197-5, 197-5]

cp10_b = [109.5-5, 109.5-5, 110-5, 110.5-5]
cp10_a = [198.5-5, 201-5, 180, 178-5]

cp11_b = [97.5-5, 99.5-5, 99.5-5, 101-5]
cp11_a = [137.5-5, 133.5-5, 134.5-5, 136-5]

cp12_b = [104.5-5, 104-5, 109.5-5, 105.5-5]
cp12_a = [154-5, 149.5-5, 146.5-5, 147-5]

cp13_b = [107.5-5, 107.5-5, 106.5-5, 106-5]
cp13_a = [153-5, 158-5, 164-5, 162-5]

yb = [np.mean(cp6_b), np.mean(cp7_b), np.mean(cp8_b), np.mean(cp9_b), np.mean(cp10_b), np.mean(cp11_b), np.mean(cp12_b), np.mean(cp13_b)]
ya = [np.mean(cp6_a), np.mean(cp7_a), np.mean(cp8_a), np.mean(cp9_a), np.mean(cp10_a), np.mean(cp11_a), np.mean(cp12_a), np.mean(cp13_a)]

# Two subplots, unpack the axes array immediately
f, (ax1, ax2) = plt.subplots(1, 2, sharey=True)
ax1.set_xticks([0, 1, 2, 3, 4, 5, 6, 7])
ax1.set_xticklabels(['6', '7', '8', '9', '10', '11', '12', '13'])

ax1.set_xlabel('number of checkpoints')
ax1.set_ylabel('distance covered (cm)')
ax1.axhline(y=np.mean(target), color='r')
ax1.plot(yb, 'bo', ya, 'ro')

cp20_b = [115-5,115-5,114-5, 114.5-5]
cp20_a = [139-5, 140-5, 143-5, 138.5-5]

cp30_b = [120.5-5, 121.5-5, 119.5-5, 121-5]
cp30_a = [129-5, 132.5-5, 137.5-5, 133-5]

cp40_b = [118.5-5, 120.5-5, 118-5, 121-5]
cp40_a = [130.5-5, 133.5-5, 132.5-5, 128-5]

cp50_b = [121-5, 121.5-5, 119.5-5, 123.5-5]
cp50_a = [136-5, 133-5, 131-5, 135-5]

cp60_b = [118.5-5, 119-5, 120-5, 121-5]
cp60_a = [130-5, 131-5, 130-5, 128.5-5]

cp70_b = [122.5-5, 122.5-5, 122.5-5, 122]
cp70_a = [131.5-5, 130.5-5, 132.5-5, 131.5-5]

cp80_b = [122-5, 122-5, 124-5, 122-5]
cp80_a = [129.5-5, 129.5-5, 129-5, 128-5]

yb = [np.mean(cp10_b), np.mean(cp20_b), np.mean(cp30_b), np.mean(cp40_b), np.mean(cp50_b), np.mean(cp60_b), np.mean(cp70_b), np.mean(cp80_b)]
ya = [np.mean(cp10_a), np.mean(cp20_a), np.mean(cp30_a), np.mean(cp40_a), np.mean(cp50_a), np.mean(cp60_a), np.mean(cp70_a), np.mean(cp80_a)]

ax2.set_xticks([0, 1, 2, 3, 4, 5, 6, 7])
ax2.set_xticklabels(['10', '20', '30', '40', '50', '60', '70', '80'])

ax2.set_xlabel('number of checkpoints')
# plt.ylabel('distance covered (cm)')
ax2.axhline(y=np.mean(target), color='r')
ax2.plot(yb, 'bo', ya, 'ro')

plt.show()