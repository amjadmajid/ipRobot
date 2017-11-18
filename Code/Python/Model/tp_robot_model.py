import math as m
# Install bms from https://github.com/kpschaper/BMSpy
import bms
from bms.signals.functions import Step
from bms.blocks.continuous import Gain, ODE, Sum, Subtraction, Product
from bms.blocks.nonlinear import Coulomb

Period = 2000

V = 2.2
R = 14.5
L = 70e-6
Jm = 1.3e-6  # estimated from python motor current graph
Jl = 0
J = Jm + Jl
No_load_speed = 1200*(2*m.pi/60)  # rad/s
No_load_current = 35e-3  # A
Rated_load_speed = 850*(2*m.pi/60)  # rad/s
Rated_load = 0.8e-3  # Nm
Rated_load_current = 80e-3  # A
Rated_voltage = 3.0

Ke = Rated_voltage / (No_load_speed)
Kt = Rated_load / (Rated_load_current - No_load_current)
Bm = Kt * No_load_current / No_load_speed
Bl = 0  # %Kt * Rated_load_current / Rated_load_speed; %0; %5e-6;
B = Bm + Bl

m = 21e-3
r = 70e-3
g = 9.81
ur = 0.015
Tr = r * ur * m * g

print 'Tr: ' + str(Tr)

Vim = Step(('Voltage Input Motor', 'Vim'), V)

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
block4 = Subtraction(Tm, Text, T)
block5 = Sum([Tm, Text], T)
block5a = Coulomb(Tm, W, Text, Tr, 2)
block6 = ODE(T, W, [1], [B, J])
block7 = Gain(W, e, Ke)
block8 = Product(Vim, Iind, Pe)
block8a = Product(Tm, W, Pm)
ds = bms.DynamicSystem(2, 1000, [block1, block2, block3, block4, block5, block5a, block6, block7, block8, block8a])

#ds.DrawModel()
r = ds.Simulate()
ds.PlotVariables([[W], [Tm, Text, T]])
ds.PlotVariables([[Iind], [Pe, Pm]])
#ds.PlotVariables([[Vim, e, Vind]]) #, [Ec]])

ds.PlotShow()