import random
import time 
import numpy as np
import matplotlib.pyplot as plt 
import matplotlib.animation as animation
import matplotlib.style as style


style.use('fivethirtyeight')
fig = plt.figure()
ax1 = fig.add_subplot(1,1,1)
ax1.set_ylim(0,4)
#fig.set_size_inches(7, 4.5)

def cnt_before(deathProb):
    """Count the step before taking it"""
    robot=0
    counter =0
    
    if random.random() < deathProb:
        counter = 1 # counting the movement 
        return (robot, counter)
        #-------- the device died here 

    counter = 1 # counting the movement     
    robot =1 # real movement 
    return (counter, robot)


def cnt_after(deathProb):
    """Count the step after taking it"""
    robot=0
    counter =0
    
    if random.random() < deathProb:
        robot = 1 # real movement 
        return (robot, counter)
        #-------- the device died here 
        
    robot = 1 # real movement     
    counter = 1      # counting the movement 
    return (counter, robot)



def cnt_alternate(deathProb):
    """alternate the counter position"""
    
    if random.random() > 0.5:
        counter, robot = cnt_after(deathProb)
    else:
        counter, robot = cnt_before(deathProb)
    return (counter, robot)



def animate_move(i):

    deathProb=0.2
    numSteps=100

    cntr_before=0
    cntr_before_rob=0

    cntr_alter=0
    cntr_alter_rob=0

    for x in range(i):
        (counter, robot) = cnt_before(deathProb)
        cntr_before+=counter
        cntr_before_rob+=robot
        (counter, robot) = cnt_alternate(deathProb)
        cntr_alter+=counter
        cntr_alter_rob+=robot
        
        ax1.clear()
        plt.plot( range(cntr_before), [1] * cntr_before , range(cntr_before_rob), [1.2] * cntr_before_rob, \
         range(cntr_alter), [1.5] * cntr_alter , range(cntr_alter_rob), [1.7] * cntr_alter_rob, \
         range(cntr_alter), [2.2] * cntr_alter , range(cntr_alter_rob), [2.2] * cntr_alter_rob )


a = animation.FuncAnimation(fig, animate_move, interval=10)
plt.show();
















