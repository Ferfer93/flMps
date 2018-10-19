import numpy as np
from math import pi
import matplotlib.pyplot as plt
from matplotlib import animation
from matplotlib.collections import EllipseCollection

#fig, ax = plt.subplots()
#file = np.loadtxt('snaps.out', dtype='f', delimiter=' ')
#for i in range(len(file)):
    #theta = file[i,3]/(2*3.141593)
    #circle = plt.Circle((file[i,1]/100.0,file[i,2]/100.0), 0.01, color=(1.0-theta, theta, 0.0))
    #ax.add_artist(circle)
#fig.show()

#fig2, ax2 =plt.subplots()
#file2 = np.loadtxt('snapsL.out', delimiter=' ')
#file2 = np.flipud(np.transpose(file2))
#plt.imshow(file2, cmap='hot')
#plt.show()
#plt.pause(-1)

data = np.loadtxt('sim_0.55.1.out', delimiter=' ')

fig, ax = plt.subplots()

Nps = 2000

ax.set_xlim(0, 102)
ax.set_ylim(0, 102)
ellcoll = EllipseCollection(0.5,0.5,1, units='x', offsets = [], transOffset=ax.transData)
ax.add_collection(ellcoll)
#ax.add_artist(circ)

def init():
    ax.set_xlim(0, 102)
    ax.set_ylim(0, 102)
    ellcoll.set_offsets(data[0:Nps,1:3])
    return ellcoll,

def update(frame):
    ellcoll.set_offsets(data[frame*Nps:(frame*1+1)*Nps,1:3])
    zrs = np.zeros(Nps)
    cols = np.transpose(data[frame*Nps:(frame*1+1)*Nps,3]/(2*pi))
    comp = np.concatenate(([1-cols],[cols],[zrs]))
    #print(len(comp))
    ellcoll.set_facecolor(np.transpose(comp))
    return ellcoll,

ani = animation.FuncAnimation(fig, update, frames=range(999),
                    init_func=init, blit=True, interval=40)

ani.save('movie2.avi')
#plt.show()
