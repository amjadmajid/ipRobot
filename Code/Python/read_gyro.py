from __future__ import division
import csv
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
    if process.returncode == 11:
        print "\n ERROR: The debugger is already in use!"
    elif process.returncode == 16:
        print "\n ERROR: The device is wrong or not connected!"
    else:
        print "\n ERROR: Unknown"
    quit(1)

output = open('output.hex', 'r')
arr = output.readlines()
# remove end-of-file record
values = arr[0:(len(arr)-1)]

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
    ang += x * 0.02
    tarr.append(ang)

print iarr

if 0:
    name = "name.csv"
    with open(name, "wb") as csv_file:
            wr = csv.writer(csv_file, quoting=csv.QUOTE_ALL)
            wr.writerow(iarr)

if 0:
    larr = []
    i = 0
    dec = 1
    for x in range(0, len(iarr) - 1):
        ans = iarr[i + 1] - iarr[i]
        # larr.append(ans)
        if ans > 0 and dec:
            larr.append(i)
            dec = 0
        elif ans < 0:
            dec = 1
        i += 1

    i = 0
    larr2 = []
    for x in range(1, len(larr)):
        ans = larr[i + 1] - larr[i]
        larr2.append(ans)
        i += 1

    # remove first minimum
    larr2 = larr2[1:len(larr2)]
    print larr2
    print sum(larr2) / float(len(larr2))




x = np.linspace(0, num_samples, num_samples)

plt.figure(1)
plt.plot(iarr)
plt.show()