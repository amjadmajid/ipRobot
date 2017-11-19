from __future__ import division
from math import pi, sin
# Install bms from https://github.com/kpschaper/BMSpy
import bms
from bms.signals.functions import Step, SignalFunction
from bms.blocks.continuous import Gain, ODE, Sum, Subtraction, Product
from bms.blocks.nonlinear import Coulomb

Period = 2000

V = 2.2
R = 14.5
L = 70e-6
Jm = 1.3e-6  # estimated from python motor current graph
Jl = 0
J = Jm + Jl
No_load_speed = 1200*(2*pi/60)  # rad/s
No_load_current = 35e-3  # A
Rated_load_speed = 850*(2*pi/60)  # rad/s
Rated_load = 0.8e-3  # Nm
Rated_load_current = 80e-3  # A
Rated_voltage = 3.0

Ke = Rated_voltage / No_load_speed
Kt = Rated_load / (Rated_load_current - No_load_current)
Bm = Kt * No_load_current / No_load_speed
Bl = 0  # %Kt * Rated_load_current / Rated_load_speed; %0; %5e-6;
B = Bm + Bl

m = 21e-3 / 2
r = 70e-3
g = 9.81
ur = 0.015
Tr = r * ur * m * g

te = 2
fs = 2000
ns = te * fs
period = 1/fs

#Vim = Step(('Voltage Input Motor', 'Vim'), V)
# Duty cycle 50%
Vim = SignalFunction(('Voltage Input Motor', 'Vim'), lambda t: V if sin((2*pi*t)/period) > 0 else 0 if sin((2*pi*t)/period) > 0 else 0.5*V)

e = bms.Variable(('Counter electromotive force', 'Cef'))
Vind = bms.Variable(('Voltage Inductor', 'Vi'))
Iind = bms.Variable(('Current Inductor', 'Ii'))
Tm = bms.Variable(('Motor torque', 'Tm'))
Text = bms.Variable(('Resistant torque', 'Tr'))
T = bms.Variable(('Torque', 'T'))
W = bms.Variable(('Rotational speed', 'w'))
Pe = bms.Variable(('Electrical power', 'Pe'))
Pm = bms.Variable(('Mechanical power', 'Pm'))

block1 = Subtraction(Vim, e, Vind)
block2 = ODE(Vind,Iind, [1], [R, L])
block3 = Gain(Iind, Tm, Kt)
block4 = Sum([Tm, Text], T)
# Coulomb = signum function i.e. returns -Text when speed > 0 (note block 4 is sum instead of substraction)
block4a = Coulomb(Tm, W, Text, Tr, 2)
block5 = ODE(T, W, [1], [B, J])
block6 = Gain(W, e, Ke)
block7 = Product(Vim, Iind, Pe)
block7a = Product(Tm, W, Pm)
ds = bms.DynamicSystem(te, ns, [block1, block2, block3, block4, block4a, block5, block6, block7, block7a])

#ds.DrawModel()
r = ds.Simulate()
ds.PlotVariables([[W], [Tm, Text, T]])
ds.PlotVariables([[Iind], [Pe, Pm]])
#ds.PlotVariables([[Vim, e, Vind]]) #, [Ec]])

ds.PlotShow()