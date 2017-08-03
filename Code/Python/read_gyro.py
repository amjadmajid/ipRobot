from __future__ import division
import subprocess
import math
import binascii
import struct
import matplotlib.pyplot as plt
import numpy as np

num_samples = 200

# Get whole lines, otherwise unhexify will crash
num_lines = int(32 * (math.ceil(num_samples*2/32))) -1
end = format(17408 + num_lines,'#04x')

process = subprocess.Popen(
    ["MSP430Flasher.exe", "-g", "-r", "[output.hex,0x4400-" + str(end) + "]", "-z", "[VCC=2200]"],
    stdin=subprocess.PIPE)

# Do not update debugger firmware!
process.communicate("n\n")

if process.returncode != 0:
    print "\n ERROR: The debugger is already in use!"
    quit(1)

output = open('output.hex', 'r')
arr = output.readlines()
# remove end-of-file record
values = arr[1:(len(arr)-1)]

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
plt.plot(warr)
plt.show()