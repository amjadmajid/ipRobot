import subprocess
from cffi import FFI
import math
import matplotlib.pyplot as plt

p = subprocess.Popen(
    ["mingw32-make.exe", "clean"],
    stdin=subprocess.PIPE)
p.wait()

p = subprocess.Popen(
    ["mingw32-make.exe"],
    stdin=subprocess.PIPE)
p.wait()

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
    float get_rand();
    
    void part_init(uint8_t num_parts);
    PartArray* get_parts();
    float rand_n(float mu, float sigma);
    float move(float dist, float ang);
    float update(float w);
    void resample();
''')

def init_part():

    # initialize srand() using time()
    pf.init_rand()

    #pf.part_init.argtypes = [c_int]
    pf.part_init(num_part)


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
        #print 'x = ' + str(particles.parts[p].x) + ', y = ' + str(particles.parts[p].y) + ', w = ' + str(particles.parts[p].w)


    plt.plot(x_arr, y_arr, 'r.')


def draw_line(x, y, t):
    r = 5  # or whatever fits you
    plt.arrow(x, y, r * math.cos(t), r * math.sin(t), head_width=2, head_length=1, fc='k', ec='k')


def plot_bot():
    plt.plot(bot[0], bot[1], 'bo')
    draw_line(bot[0], bot[1], bot[2])


def move_bot(dist, ang):
    bot[2] += ang
    bot[0] += dist*math.cos(bot[2])
    bot[1] += dist*math.sin(bot[2])


bot = [10, 70, 0]

pf.const_map_init()

plt.figure(1)

num_part = 100

init_part()

plot_maze()
plot_part()
plot_bot()


def simulate(dist, ang):
    sp = pf.move(dist, ang)
    #print 'sp = ' + str(sp)
    neff = pf.update(sp)
    #print 'neff = ' + str(neff)
    if neff < 0.5*num_part:
        pf.resample()
        print "resample!"
    move_bot(dist, ang)

    plot_maze()
    plot_part()

    plot_bot()


for i in range(2,5*8):
    plt.figure(i+1)
    simulate(1, 0)

# rotation
plt.figure(5*8+1)
simulate(1, 1.5 *math.pi)

for i in range(5*8+1, (5*8+1)+20):
    plt.figure(i + 1)
    simulate(1, 0)

# rotation
plt.figure((5*8+1) + 20 + 1)
simulate(1, 0.5 *math.pi)

for i in range((5*8+1)+ 22, (5*8+1)+42):
    plt.figure(i + 1)
    simulate(1, 0)

plt.show()
