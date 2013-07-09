/* Siconos-Kernel, Copyright INRIA 2005-2012.
 * Siconos is a program dedicated to modeling, simulation and control
 * of non smooth dynamical systems.
 * Siconos is a free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * Siconos is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Siconos; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * Contact: Vincent ACARY, siconos-team@lists.gforge.inria.fr
*/

#include "ModelingTools.hpp"
#include "SimulationTools.hpp"


#include "SlidingReducedOrderObserver.hpp"
#include "ControlSensor.hpp"
#include "ObserverFactory.hpp"

void SlidingReducedOrderObserver::initialize(const Model& m)
{
  if (!_C)
  {
    RuntimeException::selfThrow("SlidingReducedOrderObserver::initialize - you have to set C before initializing the Observer");
  }
  else
  {
    Observer::initialize(m);
  }
  DynamicalSystemsGraph& originalDSG0 = *m.nonSmoothDynamicalSystem()->topology()->dSG(0);
  DynamicalSystemsGraph::VDescriptor originaldsgVD;
  if (!_DS) // No DynamicalSystem was given
  {
    // We can only work with FirstOrderNonLinearDS, FirstOrderLinearDS and FirstOrderLinearTIDS
    // We can use the Visitor mighty power to check if we have the right type
    DynamicalSystem& observedDS = *_sensor->getDS();
    Type::Siconos dsType;
    dsType = Type::value(observedDS);
    // create the DS for the controller
    // if the DS we use is different from the DS we are controlling
    // when we want for instant to see how well the controller behaves
    // if the plant model is not exact, we can use the setSimulatedDS
    // method
    if (dsType == Type::FirstOrderLinearDS)
    {
      _DS.reset(new FirstOrderLinearDS(static_cast<FirstOrderLinearDS&>(observedDS)));
    }
    else if (dsType == Type::FirstOrderLinearTIDS)
    {
      _DS.reset(new FirstOrderLinearTIDS(static_cast<FirstOrderLinearTIDS&>(observedDS)));
    }
    else
      RuntimeException::selfThrow("SlidingReducedOrderObserver is not yet implemented for system of type" + dsType);

    // is it controlled ?
    originaldsgVD = originalDSG0.descriptor(_sensor->getDS());
  }
  else
  {
    // is it controlled ?
    originaldsgVD = originalDSG0.descriptor(_DS);
  }

  // Initialize with the guessed state
  _DS->setX0Ptr(_xHat);

  _e.reset(new SiconosVector(_C->size(0)));
  _y.reset(new SiconosVector(_C->size(0)));

  double t0 = m.t0();
  double h = m.simulation()->timeDiscretisation()->currentTimeStep();
  double T = m.finalT() + h;
  _model.reset(new Model(t0, T));
  _model->nonSmoothDynamicalSystem()->insertDynamicalSystem(_DS);

  // Add the necessary properties
  DynamicalSystemsGraph& DSG0 = *_model->nonSmoothDynamicalSystem()->topology()->dSG(0);
  DynamicalSystemsGraph::VDescriptor dsgVD = DSG0.descriptor(_DS);
  // Observer part
  DSG0.L[dsgVD] = _L;
  DSG0.e[dsgVD] = _e;

  // Was the original DynamicalSystem controlled ?
  if (originalDSG0.B.hasKey(originaldsgVD))
  {
    DSG0.B[dsgVD] = originalDSG0.B[originaldsgVD];
    assert(originalDSG0.u[originaldsgVD] && "A DynamicalSystem is controlled but its control input has not been initialized yet");
    DSG0.u[dsgVD] = originalDSG0.u[originaldsgVD];
  }

  // all necessary things for simulation
  _simulation.reset(new TimeStepping(_td, 0));
  _integrator.reset(new ZeroOrderHold(_DS));
  _simulation->insertIntegrator(_integrator);
  _model->initialize(_simulation);

  // initialize error
  *_y = _sensor->y();
}

void SlidingReducedOrderObserver::process()
{
  if (!_pass)
    _pass = true;
  else
  {
    // get measurement from sensor
    const SiconosVector& y = _sensor->y();
    // update the current measured value
    *_y = y;

    // First pass, set _e to 0, integrate the system
    // and get the innovation term
    _e->zero();
    _simulation->computeOneStep();

    // e = C*xhat_k - y_{k+1}
    prod(*_C, _DS->getx(), *_e);
    *_e -= *_y;

    // Second pass, now we update the state
    //
    // But first we need to reset the state to the
    // previous value (at t_k)
    _DS->setX(*_DS->xMemory()->getSiconosVector(0));
    // integrate with the new innovation term
    _simulation->computeOneStep();

    // We can go one step forward
    _simulation->nextStep();

    *_xHat = _DS->getx();
  }
}

void SlidingReducedOrderObserver::setL(const SiconosMatrix& L)
{
    _L.reset(new SimpleMatrix(L));
}

AUTO_REGISTER_OBSERVER(SLIDING_REDUCED_ORDER, SlidingReducedOrderObserver);