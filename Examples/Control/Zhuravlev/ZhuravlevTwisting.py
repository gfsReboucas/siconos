#!/usr/bin/env python

# Siconos-sample, Copyright INRIA 2005-2011.
# Siconos is a program dedicated to modeling, simulation and control
# of non smooth dynamical systems.
# Siconos is a free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
# Siconos is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Siconos; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
#
# Contact: Vincent ACARY, siconos-team@lists.gforge.fr
#


import matplotlib.pyplot as plt
from numpy import array, eye, empty, zeros, savetxt
import numpy as np
from Siconos.Kernel import FirstOrderLinearDS, FirstOrderLinearTIR, RelayNSL, \
NonSmoothDynamicalSystem, Model, TimeDiscretisation, TimeStepping, EulerMoreauOSI, \
Interaction, Relay
from math import ceil


from ZhuravlevTwistingR import ZhuravlevTwistingR


# variables
t0 = 0.0   # start time
T = 10.0     # end time
h = 1.0e-3   # time step
numInter = 2
ninter = 2
theta = 0.5
alpha = .01
N = ceil((T-t0)/h)

# matrices
A = zeros((2,2))
A[0, 1] = 1

x0 = array([1.,10.])
B = 500*array([[alpha,1-alpha],[-(1-alpha),alpha]])
C = eye(2)
D = zeros((2,2))

# dynamical systems
process = FirstOrderLinearDS(x0, A)
myProcessRelation = ZhuravlevTwistingR(C,B)

myNslaw = RelayNSL(2)
myNslaw.display()

myProcessInteraction = Interaction(ninter, myNslaw,
        myProcessRelation)
myNSDS = NonSmoothDynamicalSystem()
myNSDS.insertDynamicalSystem(process)
myNSDS.link(myProcessInteraction,process)


filippov = Model(t0,T)
filippov.setNonSmoothDynamicalSystemPtr(myNSDS)

td = TimeDiscretisation(t0, h)
s = TimeStepping(td)

myIntegrator = EulerMoreauOSI(process, theta)
s.insertIntegrator(myIntegrator)


#TODO python <- SICONOS_RELAY_LEMKE
# access dparam

osnspb = Relay()
s.insertNonSmoothProblem(osnspb)
s.setComputeResiduY(True)
s.setComputeResiduR(True)

filippov.initialize(s);

# matrix to save data
dataPlot = empty((N+1,5))
control = empty((N+1,))
dataPlot[0, 0] = t0
dataPlot[0, 1:3] = process.x()
dataPlot[0, 3] = myProcessInteraction.lambda_(0)[0]
dataPlot[0, 4] = myProcessInteraction.lambda_(0)[1]
# time loop
k = 1
while(s.hasNextEvent()):
     s.newtonSolve(1e-14, 30)
     dataPlot[k, 0] = s.nextTime()
     dataPlot[k, 1] = process.x()[0]
     dataPlot[k, 2] = process.x()[1]
     dataPlot[k, 3] = myProcessInteraction.lambda_(0)[0]
     dataPlot[k, 4] = myProcessInteraction.lambda_(0)[1]
     control[k] = process.r()[1]
     k += 1
     s.nextStep()
     #print s.nextTime()

# save to disk
np.savetxt('output.txt', dataPlot)
# plot interesting stuff
plt.subplot(411)
plt.title('s')
plt.plot(dataPlot[:,0], dataPlot[:,1])
plt.grid()
plt.subplot(412)
plt.title('v')
plt.plot(dataPlot[:,0], dataPlot[:,2])
plt.grid()
plt.subplot(413)
plt.plot(dataPlot[:,0], dataPlot[:,3])
plt.title('lambda1')
plt.grid()
plt.subplot(414)
plt.plot(dataPlot[:,0], dataPlot[:,4])
plt.title('lambda2')
plt.grid()
plt.show()

plt.plot(dataPlot[:,1], dataPlot[:,2])
plt.xlabel('s')
plt.xlabel('v')
plt.grid()
plt.show()

plt.plot(dataPlot[:,3], dataPlot[:,4])
plt.xlabel('lambda1')
plt.xlabel('lambda2')
plt.grid()
plt.show()

pos = np.abs(dataPlot[:,1])
velocity = (1-myProcessRelation._kappa*np.sign(dataPlot[:,1]*dataPlot[:,2]))*dataPlot[:, 2]*np.sign(dataPlot[:,1])

plt.subplot(311)
plt.title('position')
plt.plot(dataPlot[:,0], pos)
plt.grid()
plt.subplot(312)
plt.title('velocity')
plt.plot(dataPlot[:,0], velocity)
plt.grid()
plt.subplot(313)
plt.title('control input')
plt.plot(dataPlot[:,0], control)
plt.grid()
plt.show()

indx = np.nonzero(dataPlot[:, 0]>30)
ttt = dataPlot[indx, 0].flatten()

plt.subplot(311)
plt.title('position')
plt.plot(ttt, pos[indx])
plt.grid()
plt.subplot(312)
plt.title('velocity')
plt.plot(ttt, velocity[indx])
plt.grid()
plt.subplot(313)
plt.title('control input')
plt.plot(ttt, control[indx])
plt.grid()
plt.show()
