from __future__ import division
import math as m
import numpy as np
import scipy as sp
import matplotlib.pyplot as plt

plt.close('all')

# Solar panel
s_vout = 2.0 #V
s_iout = 2 * m.pow(10, -3) #A

# Harvester
# Note cold start is not taken into account, assume Vbat > 1.8V
#mppt = 0.8
hv_bo_eff = 0.9
hv_vbat_min = 2.1
hv_vbat_max = 4.2
hv_bu_eff = 0.9
hv_vout = 2.2

# Capacitor
c_ileak = 5 * m.pow(10, -9) # A
c_cap = 22 * m.pow(10, -3)
c_vmax = 4.5

# Motor control
mot_freq = 80
mot_num_steps = 20

# Power consumption components at 2.2v
i_wisp = 165 * m.pow(10, -6) # for 8Mhz
i_io_exp = 5 * m.pow(10, -6)
i_drv = 1.2 * m.pow(10, -3)
i_mot = 40 * m.pow(10, -3)

i_total = i_wisp + i_io_exp + 2*(i_drv + i_mot)

Ps_in = s_vout*s_iout
Pb_charge = Ps_in*hv_bo_eff
E_cap = 0.5 * c_cap * (m.pow(hv_vbat_max, 2) - m.pow(hv_vbat_min, 2))
print 'E_cap: ' + str(E_cap) + ' J'

# Assume worst case leak for now!
Pc_leak = c_ileak * hv_vbat_max
t_charge = E_cap / (Pb_charge - Pc_leak)
# Is the leakage current relevant after the cap is "fully" charged??
# Since boost converter will continue to be enabled while buck converter is active.
print 't_charge: ' + str(t_charge) + ' s'

P_discharge = (hv_vout * i_total) / hv_bu_eff
print 'P_dicharge per Mot: ' + str(P_discharge * m.pow(10, 3) / 2) + ' mW'
t_discharge = E_cap / P_discharge
print 't_dicharge: ' + str(t_discharge) + ' s'


##

m_tot = 16 * m.pow(10, -3)  # total weight = 16 gram
r = 5 * m.pow(10, -3) # wheel radius = 5 mm
mot_eff = 55
omega = mot_freq / mot_num_steps  # rev/s
T_actual = 0.14 * m.pow(10, -3)

#T_actual = (m_tot * ((a) * r))
P_mech = T_actual * omega * (mot_eff/100)

print 'P_mech: ' + str(P_mech * pow(10, 3)) + ' mW'
# print 'P_elec: ' + str(P_elec * pow(10, 3)) + ' mW'

# assume initial speed is zero
dist_cov = 2 * m.pi * r * (omega) * t_discharge
print 'dist_cov: ' + str(dist_cov * 100) + ' cm'

u = 0.02
g = 9.81
F_roll = u * (m_tot * g)
T_roll = F_roll * r

print 'T_roll: ' + str(T_roll * m.pow(10, 3)) + ' mNm'

##

#a = np.array([1 ,2 ,3 ,4 ])
#b = np.array([5,6,7,8])

#print a.dot(b)

#plt.figure()
#plt.plot(a)
#plt.figure()
#plt.plot(b)
#plt.show()