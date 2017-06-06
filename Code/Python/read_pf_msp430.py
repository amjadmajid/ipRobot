import subprocess
import binascii
import struct
from cffi import FFI
import matplotlib.pyplot as plt

ffi = FFI()
pf = ffi.dlopen('./particle_filter.dll')

ffi.cdef('''
    typedef struct Wall {
        uint8_t x1;
        uint8_t x2;
        uint8_t y1;
        uint8_t y2;
    }Wall;

    typedef struct ConstMap {
        uint8_t x_size;
        uint8_t y_size;
        uint8_t num_walls;
        struct Wall walls[31];
    }ConstMap;

    void const_map_init();
    uint8_t is_wall(uint8_t x, uint8_t y);
''')


def plot_maze():
    x_arr = []
    y_arr = []

    for y in range(0, 120):
        for x in range(0, 120):
            if pf.is_wall(x, y):
                x_arr.append(x)
                y_arr.append(y)

    plt.plot(x_arr, y_arr, 'k.')

    plt.ylim([0, 120])
    plt.xlim([0, 120])

    # plt.show()
    # pf.init_rand()
    # print (pf.get_rand())

##


# 3x uint8 + 30*4 uint8
cm_start = '0x53A2'
cm_stop = '0x5420'

# num_parts -> uint8 and particles (250) -> 4*floats
pa_start = '0x4400'
pa_stop = '0x53A1'  # 0x539E

# p = subprocess.Popen(
#     ["MSP430Flasher.exe", "-n", "MSP430FR5969", "-z", "[VCC]"],
#     stdin=subprocess.PIPE)
#
# p.communicate("n\n")
# p.wait()
#
# raw_input("Press Enter to continue...")

p = subprocess.Popen(
    ["MSP430Flasher.exe", "-g", "-r", "[output.hex,0x4400-0x5420]", "-z", "[VCC=2200]"],
    stdin=subprocess.PIPE)

p.communicate("n\n")
p.wait()

output = open('output.hex', 'r')
arr = output.readlines()
# remove end of file record
values = arr[0:len(arr)-1]

#for x in range(0, len(values)-1):
#    print values[x]

# remove intel hex record structure and make one long data string
ls = ''
for line in range(0, len(values)-1):
    s = values[line][9:41]
    ls = ls + s

cnt = 0
s = ls[0:2]
s = binascii.unhexlify(s)
num_part = struct.unpack('<B', s)[0]
cnt = cnt + 4 # discard the extra 00
print 'num part: ' + str(num_part)

x_arr = []
y_arr = []
t_arr = []
w_arr = []

for x in range(0, num_part):
    for y in range(0, 4):
        s = ls[cnt: cnt + 8]
        s = binascii.unhexlify(s)
        s = struct.unpack('<f', s)[0]
        if(y == 0):
            x_arr.append(s)
        elif(y == 1 ):
            y_arr.append(s)
        elif(y == 2):
            t_arr.append(s)
        elif(y == 3):
            w_arr.append(s)
        cnt = cnt + 8


pf.const_map_init()

plt.figure(1)

plot_maze()

plt.plot(x_arr, y_arr, 'r.')

print w_arr

plt.show()