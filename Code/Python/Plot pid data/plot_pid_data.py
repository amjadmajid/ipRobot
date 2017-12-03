from __future__ import division
import csv
import subprocess
import math
import binascii
import struct
import matplotlib.pyplot as plt
import numpy as np
from scipy.signal import savgol_filter


def read_fram():

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

    return iarr


def write_list_csv(name, dlist):
    with open(name, "wb") as csv_file:
        wr = csv.writer(csv_file, quoting=csv.QUOTE_ALL)
        wr.writerow(dlist)


def read_list_csv(name):
    with open(name, 'rb') as csvfile:
        data = csv.reader(csvfile)
        for x in data:
            dlist = map(int, x)
        return dlist


def find_minimum(dlist):
    larr = []
    i = 0
    dec = 1
    for x in range(0, len(dlist) - 1):
        ans = dlist[i + 1] - dlist[i]
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

dir = "pid_data_17-10/"

if 0:

    data = read_fram()
    write_list_csv(dir + "r2_straight_kp0.18_tu20_200.csv", data)

    plt.figure(1)
    plt.plot(data)
    plt.show()

if 1:
    data1 = read_list_csv("pid_data/straight_kp010_osc_200.csv")
    data2 = read_list_csv("pid_data/straight_kp012_osc_200.csv")
    data3 = read_list_csv("pid_data/straight_kp013_osc_200.csv")
    data4 = read_list_csv("pid_data/straight_kp014_osc_200.csv")

    plt.figure(1)
    x = np.linspace(0, 2, 200)
    plt.plot(x, data1, ':')
    plt.plot(x, data2, '-.')
    plt.plot(x, data3, '--')
    plt.plot(x, data4, '-')

    plt.ylabel('Yaw-rate (deg/s)', fontsize=12)
    plt.xlabel('Time (s)', fontsize=12)
    plt.xticks(fontsize=12)
    plt.yticks(fontsize=12)
    plt.legend(('Ku: 0.10', 'Ku: 0.12','Ku: 0.13', 'Ku: 0.14'), loc='upper left', fontsize=12)

    plt.figure(2)
    # avg = savgol_filter(data3, 5, 2)  # window size 51, polynomial order 3
    data5 = read_list_csv("pid_data/straight_kp013_tu20_200.csv")

    plt.plot(x, data3, ':')
    plt.plot(x, data5, '-.')
    plt.ylabel('Yaw-rate (deg/s)', fontsize=12)
    plt.xlabel('Time (s)', fontsize=12)
    plt.xticks(fontsize=12)
    plt.yticks(fontsize=12)
    plt.legend(('Ku: 0.13, Ki = 0, Kd = 0', 'Kp: 0.078, Ki = 1.3, Kd = 0.0325'), loc='upper left', fontsize=12)

    plt.show()