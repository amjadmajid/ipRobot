from ctypes import *
import matplotlib.pyplot as plt

pf = cdll.LoadLibrary('particle_filter.dll')
pf.const_map_init()

x_arr = []
y_arr = []

# Set argument and return types
pf.is_wall.argtypes = [c_int, c_int]
pf.is_wall.restype = c_int

for y in range(0, 120):
    for x in range(0, 120):
        if pf.is_wall(x, y):
            x_arr.append(x)
            y_arr.append(y)

plt.plot(x_arr, y_arr, 'k.')

plt.ylim([0, 120])
plt.xlim([0, 120])

#plt.show()
#pf.init_rand()
#print (pf.get_rand())

##


class Particle(Structure):
    _fields_ = [("x", c_uint8),
                ("y", c_uint8),
                ("wgt", c_uint8)]


class PartArray(Structure):
    _fields_ = [("num_parts", c_uint8),
                ("parts", Particle * 250)]

# initialize srand() using time()
pf.init_rand()

pf.part_init.argtypes = [c_int]
pf.part_init(250)

# Set return type
pf.get_parts.restype = POINTER(PartArray)

particles = pf.get_parts().contents

x_arr = []
y_arr = []

for p in range(0, particles.num_parts):
    x_arr.append(particles.parts[p].x)
    y_arr.append(particles.parts[p].y)
    print 'x = ' + str(particles.parts[p].x) + ', y = ' + str(particles.parts[p].y)

plt.plot(x_arr, y_arr, 'r.')

plt.show()