import ctypes
import matplotlib.pyplot as plt

lib_const_map = ctypes.cdll.LoadLibrary('const_map.dll')
lib_const_map.const_map_init()

x_arr = []
y_arr = []

# Set argument and return types
lib_const_map.is_wall.argtypes = [ctypes.c_int, ctypes.c_int]
lib_const_map.is_wall.restype = ctypes.c_int

for y in range(0, 120):
    for x in range(0, 120):
        if lib_const_map.is_wall(x,y):
            x_arr.append(x)
            y_arr.append(y)

plt.plot(x_arr, y_arr, 'k.')

plt.ylim([0, 120])
plt.xlim([0, 120])

plt.show()