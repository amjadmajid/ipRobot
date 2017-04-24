import os
from subprocess import Popen
from cffi import FFI
import math
import matplotlib.pyplot as plt

fn = os.path.join(os.path.dirname(__file__), 'make.bat')
bat = Popen(fn)
bat.wait()

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

    #plt.show()
    #pf.init_rand()
    #print (pf.get_rand())

##

ffi.cdef('''
    typedef struct Particle {
        float x;
        float y;
        float t;
        float w;
    }Particle;
    
    typedef struct PartArray {
        uint8_t num_parts;
        struct Particle parts[250];
    }PartArray;
    
    void init_rand();
    uint16_t get_rand();
    
    void part_init(uint8_t num_parts);
    PartArray* get_parts();
    float gaussian(float mean, float stddev);
    void motion_model(uint8_t left, uint8_t right);
    uint8_t move(uint8_t dist, float ang);
    void resample(uint8_t dcnt);
''')

def init_part():

    # initialize srand() using time()
    pf.init_rand()

    #pf.part_init.argtypes = [c_int]
    pf.part_init(240)


def plot_part():

    particles = pf.get_parts()

    x_arr = []
    y_arr = []
    w_arr = []

    for p in range(0, particles.num_parts):
        if particles.parts[p].w != 0.0:
            x_arr.append(particles.parts[p].x)
            y_arr.append(particles.parts[p].y)
            w_arr.append(particles.parts[p].w)
        # print 'x = ' + str(particles.parts[p].x) + ', y = ' + str(particles.parts[p].y) + ', w = ' + str(particles.parts[p].w)

    plt.plot(x_arr, y_arr, 'r.')
    print len(w_arr)

pf.const_map_init()

plt.figure(1)
plt.subplot(221)

init_part()

plot_maze()
plot_part()

plt.subplot(222)

print 'dead = ' + str(pf.move(10, 0.5 * math.pi))

plot_maze()
plot_part()

plt.subplot(223)

print 'dead = ' + str(pf.move(10, 0.5 * math.pi))

plot_maze()
plot_part()

plt.subplot(224)

print 'dead = ' + str(pf.move(10, 0.5 * math.pi))

plot_maze()
plot_part()

plt.show()