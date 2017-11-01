import math as m
import numpy as np
import matplotlib.pyplot as plt


def motor_const():
    Vm = 3
    Ra = 14.5
    I = 38 * m.pow(10, -3)
    Omega = 240 # +/- 35 rpm
    Ve = Vm - (I*Ra)
    print Ve
    Ke = Ve / Omega
    print Ke

V = 2.2
Fg = 9.81
r = 1.4 * m.pow(10, -2)
us = 0.1
m = 23 * m.pow(10, -3)

def p_min():
    Tl = us * Fg * r
    Pm = Tl * omega

p_min()