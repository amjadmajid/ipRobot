from __future__ import division
import subprocess
import math
import binascii
import struct
import matplotlib.pyplot as plt
import numpy as np

num_samples = 200;

#end = format(17408 + num_samples,'#04x')
#print end

process = subprocess.Popen(
    ["MSP430Flasher.exe", "-g", "-r", "[output.hex,0x4400-0xBB80]", "-z", "[VCC=2200]"],
    stdin=subprocess.PIPE)

process.communicate("n\n")

output = open('output.hex', 'r')
arr = output.readlines()
max = int(math.ceil(num_samples/8+1))
values = arr[1:max]

#for line in values:
#    print line

iarr = []

for line in values:
    for x in range(1,9):
        s = line[x * 4 + 5:x * 4 + 9]
        s = binascii.unhexlify(s)
        s = struct.unpack('<h', s)[0]
        iarr.append(s)

iarr = iarr[:num_samples]

warr = []
warr = [x / 131 for x in iarr]

ang = 0.0
tarr = []
for x in warr:
    ang += x * 0.01
    tarr.append(ang)

print tarr

x = np.linspace(0, num_samples, num_samples)

plt.figure(1)
plt.plot(x, tarr)
plt.show()