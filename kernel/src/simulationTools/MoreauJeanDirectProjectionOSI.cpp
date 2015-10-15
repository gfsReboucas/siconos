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
#include "MoreauJeanDirectProjectionOSI.hpp"
#include "Simulation.hpp"
#include "Model.hpp"
#include "NewtonEulerDS.hpp"
#include "LagrangianDS.hpp"
#include "CxxStd.hpp"
//#define STANDARD_ACTIVATION
#define FIRSTWAY_ACTIVATION
//#define SECONDWAY_ACTIVATION
//#define QFREE_ACTIVATION


//#define DEBUG_MESSAGES
//#define DEBUG_STDOUT
//#define DEBUG_WHERE_MESSAGES
#include <debug.h>

#define SICONOS_MPC_DEFAULT_ACTIVATION_POS_THRESHOLD 1.e-7
#define SICONOS_MPC_DEFAULT_ACTIVATION_VEL_THRESHOLD 0.0
#define SICONOS_MPC_DEFAULT_DEACTIVATION_POS_THRESHOLD 1.e-7
#define SICONOS_MPC_DEFAULT_DEACTIVATION_VEL_THRESHOLD 0.0


MoreauJeanDirectProjectionOSI::MoreauJeanDirectProjectionOSI(double theta) : MoreauJeanOSI(theta)
{
  integratorType = OSI::MOREAUDIRECTPROJECTIONOSI;
  _deactivateYPosThreshold = SICONOS_MPC_DEFAULT_DEACTIVATION_POS_THRESHOLD;
  _deactivateYVelThreshold = SICONOS_MPC_DEFAULT_DEACTIVATION_VEL_THRESHOLD;
  _activateYPosThreshold =   SICONOS_MPC_DEFAULT_ACTIVATION_POS_THRESHOLD;
  _activateYVelThreshold =   SICONOS_MPC_DEFAULT_ACTIVATION_VEL_THRESHOLD;
}

MoreauJeanDirectProjectionOSI::MoreauJeanDirectProjectionOSI(SP::DynamicalSystem ds , double d): MoreauJeanOSI(ds , d)
{
  integratorType = OSI::MOREAUDIRECTPROJECTIONOSI;
  _deactivateYPosThreshold = SICONOS_MPC_DEFAULT_DEACTIVATION_POS_THRESHOLD;
  _deactivateYVelThreshold = SICONOS_MPC_DEFAULT_DEACTIVATION_VEL_THRESHOLD;
  _activateYPosThreshold =   SICONOS_MPC_DEFAULT_ACTIVATION_POS_THRESHOLD;
  _activateYVelThreshold =   SICONOS_MPC_DEFAULT_ACTIVATION_VEL_THRESHOLD;
}

MoreauJeanDirectProjectionOSI::MoreauJeanDirectProjectionOSI(double theta, double gamma) : MoreauJeanOSI(theta, gamma)
{
  integratorType = OSI::MOREAUDIRECTPROJECTIONOSI;
  _deactivateYPosThreshold = SICONOS_MPC_DEFAULT_DEACTIVATION_POS_THRESHOLD;
  _deactivateYVelThreshold = SICONOS_MPC_DEFAULT_DEACTIVATION_VEL_THRESHOLD;
  _activateYPosThreshold =   SICONOS_MPC_DEFAULT_ACTIVATION_POS_THRESHOLD;
  _activateYVelThreshold =   SICONOS_MPC_DEFAULT_ACTIVATION_VEL_THRESHOLD;
}

MoreauJeanDirectProjectionOSI::MoreauJeanDirectProjectionOSI(SP::DynamicalSystem ds , double d, double gamma): MoreauJeanOSI(ds , d, gamma)
{
  integratorType = OSI::MOREAUDIRECTPROJECTIONOSI;
  _deactivateYPosThreshold = SICONOS_MPC_DEFAULT_DEACTIVATION_POS_THRESHOLD;
  _deactivateYVelThreshold = SICONOS_MPC_DEFAULT_DEACTIVATION_VEL_THRESHOLD;
  _activateYPosThreshold =   SICONOS_MPC_DEFAULT_ACTIVATION_POS_THRESHOLD;
  _activateYVelThreshold =   SICONOS_MPC_DEFAULT_ACTIVATION_VEL_THRESHOLD;
}


void MoreauJeanDirectProjectionOSI::initialize()
{

  MoreauJeanOSI::initialize();

  ConstDSIterator itDS;
  for (itDS = OSIDynamicalSystems->begin(); itDS != OSIDynamicalSystems->end(); ++itDS)
  {
    Type::Siconos dsType = Type::value(**itDS);
    if (dsType == Type::LagrangianDS || dsType == Type::LagrangianLinearTIDS)
    {
      SP::LagrangianDS d = std11::static_pointer_cast<LagrangianDS> (*itDS);
      d->allocateWorkVector(DynamicalSystem::qtmp, d->getNdof());
    }
    else if (dsType == Type::NewtonEulerDS)
    {
      SP::NewtonEulerDS d = std11::static_pointer_cast<NewtonEulerDS>(*itDS);
      d->allocateWorkVector(DynamicalSystem::qtmp, d->q()->size());
    }
    else
    {
      RuntimeException::selfThrow("MoreauJeanDirectProjectionOSI::initialize() - DS not of the right type");
    }
  }
}

void MoreauJeanDirectProjectionOSI::computeFreeState()
{
  MoreauJeanOSI::computeFreeState();


  // Compute qfree
  //   double h = simulationLink->timeStep();

  //   DSIterator it; // Iterator through the set of DS.

  //   SP::DynamicalSystem ds; // Current Dynamical System.
  //   SP::SiconosMatrix W; // W MoreauJeanOSI matrix of the current DS.
  //   Type::Siconos dsType ; // Type of the current DS.
  //   for (it=OSIDynamicalSystems->begin(); it!= OSIDynamicalSystems->end(); ++it)
  //   {
  //     ds = *it; // the considered dynamical system
  //     dsType = Type::value(*ds); // Its type
  //     if (dsType == Type::LagrangianLinearTIDS ||
  //         dsType == Type::LagrangianDS)
  //     {

  //       SP::LagrangianDS d = std11::static_pointer_cast<LagrangianDS> (ds);
  //       SP::SiconosVector vfree = d->workspace(DynamicalSystem::free);
  //       SP::SiconosVector vold = d->velocityMemory()->getSiconosVector(0);
  //       SP::SiconosVector qold = d->qMemory()->getSiconosVector(0);
  //       SP::SiconosVector q = d->q();
  // #ifdef DEBUG_MESSAGES
  //       DEBUG_PRINT("MoreauJeanDirectProjectionOSI::computeFreeState() q before.\n");
  //       q->display();
  // #endif
  //       // *q = *qold + h*(theta * *v +(1.0 - theta)* *vold)
  //       double coeff = h*_theta;
  //       scal(coeff, *vfree, *q) ; // q = h*theta*v
  //       coeff = h*(1-_theta);
  //       scal(coeff,*vold,*q,false); // q += h(1-theta)*vold
  //       *q += *qold;
  // #ifdef DEBUG_MESSAGES
  //       DEBUG_PRINT("MoreauJeanDirectProjectionOSI::computeFreeState() q after (qfree).\n");
  //       q->display();
  // #endif
  //     }
  //     else if (dsType == Type::NewtonEulerDS)
  //     {
  //       SP::NewtonEulerDS d = std11::static_pointer_cast<NewtonEulerDS> (ds);
  //       SP::SiconosVector vfree = d->workspace(DynamicalSystem::free);
  //       SP::SiconosMatrix T = d->T();
  //       SP::SiconosVector dotq = d->dotq();
  //       prod(*T,*vfree,*dotq,true);
  //       SP::SiconosVector q = d->q();

  //       //  -> get previous time step state
  //       SP::SiconosVector dotqold = d->dotqMemory()->getSiconosVector(0);
  //       SP::SiconosVector qold = d->qMemory()->getSiconosVector(0);
  //       // *q = *qold + h*(theta * *v +(1.0 - theta)* *vold)
  //       double coeff = h*_theta;
  //       scal(coeff, *dotq, *q) ; // q = h*theta*v
  //       coeff = h*(1-_theta);
  //       scal(coeff,*dotqold,*q,false); // q += h(1-theta)*vold
  //       *q += *qold;

  //     }
  //   }
}

#ifdef STANDARD_ACTIVATION
bool MoreauJeanDirectProjectionOSI::addInteractionInIndexSet(SP::Interaction inter, unsigned int i)
{
  return MoreauJeanOSI::addInteractionInIndexSet(inter, i);
}

bool MoreauJeanDirectProjectionOSI::removeInteractionInIndexSet(SP::Interaction inter, unsigned int i)
{
  return MoreauJeanOSI::removeInteractionInIndexSet(inter, i);
}
#endif


#ifdef FIRSTWAY_ACTIVATION
bool MoreauJeanDirectProjectionOSI::addInteractionInIndexSet(SP::Interaction inter, unsigned int i)
{

  assert(i == 1);
  double h = simulationLink->timeStep();
  double y = (inter->y(i - 1))->getValue(0); // for i=1 y(i-1) is the position
  double yDot = (inter->y(i))->getValue(0); // for i=1 y(i) is the velocity
  double gamma = 1.0 / 2.0;
  if (_useGamma)
  {
    gamma = _gamma;
  }
  DEBUG_PRINTF("MoreauJeanOSI::addInteractionInIndexSet yref=%e, yDot=%e, y_estimated=%e.\n", y, yDot, y + gamma * h * yDot);
  y += gamma * h * yDot;


  DEBUG_PRINTF("MoreauJeanDirectProjectionOSI::addInteractionInIndexSet yref=%e, yDot=%e.\n", y, yDot);

  DEBUG_PRINTF("MoreauJeanDirectProjectionOSI::addInteractionInIndexSet  _activateYPosThreshold =%e, _activateYVelThreshold=%e\n",
               _activateYPosThreshold ,
               _activateYVelThreshold);

  assert(!isnan(y));
#ifdef DEBUG_MESSAGES
  if (y <= _activateYPosThreshold)
    DEBUG_PRINT("MoreauJeanDirectProjectionOSI::addInteractionInIndexSet ACTIVATE.\n");
#endif
  return (y <= _activateYPosThreshold);
}

bool MoreauJeanDirectProjectionOSI::removeInteractionInIndexSet(SP::Interaction inter, unsigned int i)

{
  assert(i == 1);
  double h = simulationLink->timeStep();
  double y = (inter->y(i - 1))->getValue(0); // for i=1 y(i-1) is the position
  double yDot = (inter->y(i))->getValue(0); // for i=1 y(i) is the velocity
  double gamma = 1.0 / 2.0;
  if (_useGamma)
  {
    gamma = _gamma;
  }
  DEBUG_PRINTF("MoreauJeanDirectProjectionOSI::removeInteractionInIndexSet yref=%e, yDot=%e .\n", y, yDot);
  y += gamma * h * yDot;

  DEBUG_PRINTF("MoreauJeanDirectProjectionOSI::removeInteractionInIndexSet  _deactivateYPosThreshold =%e, _deactivateYVelThreshold=%e\n",
               _deactivateYPosThreshold ,
               _deactivateYVelThreshold);

  assert(!isnan(y));
#ifdef DEBUG_MESSAGES
  if (y > _deactivateYPosThreshold && yDot >= _deactivateYVelThreshold)
    DEBUG_PRINT("MoreauJeanDirectProjectionOSI::removeInteractionInIndexSet DEACTIVATE.\n");
#endif
  return (y > _deactivateYPosThreshold && yDot >= _deactivateYVelThreshold);
}

#endif



#ifdef SECONDWAY_ACTIVATION
bool MoreauJeanDirectProjectionOSI::addInteractionInIndexSet(SP::Interaction inter, unsigned int i)
{

  assert(i == 1);
  double y = (inter->y(i - 1))->getValue(0); // for i=1 y(i-1) is the position
#ifdef DEBUG_MESSAGES
  double yDot = (inter->y(i))->getValue(0); // for i=1 y(i) is the velocity
#endif
  DEBUG_PRINTF("MoreauJeanDirectProjectionOSI::addInteractionInIndexSet yref=%e, yDot=%e.\n", y, yDot);

  DEBUG_PRINTF("MoreauJeanDirectProjectionOSI::addInteractionInIndexSet  _activateYPosThreshold =%e, _activateYVelThreshold=%e\n",
               _activateYPosThreshold ,
               _activateYVelThreshold);

  assert(!isnan(y));

  if (y <= _activateYPosThreshold)
    DEBUG_PRINT("MoreauJeanDirectProjectionOSI::addInteractionInIndexSet ACTIVATE.\n");
  return (y <= _activateYPosThreshold);
}

bool MoreauJeanDirectProjectionOSI::removeInteractionInIndexSet(SP::Interaction inter, unsigned int i)

{
  assert(i == 1);
  double y = (inter->y(i - 1))->getValue(0); // for i=1 y(i-1) is the position
  double yDot = (inter->y(i))->getValue(0); // for i=1 y(i) is the velocity
  double lambda = (inter->lambda(i))->getValue(0); // for i=1 y(i) is the velocity

  DEBUG_PRINTF("MoreauJeanDirectProjectionOSI::removeInteractionInIndexSet yref=%e, yDot=%e .\n", y, yDot);

  DEBUG_PRINTF("MoreauJeanDirectProjectionOSI::removeInteractionInIndexSet  _deactivateYPosThreshold =%e, _deactivateYVelThreshold=%e\n",
               _deactivateYPosThreshold ,
               _deactivateYVelThreshold);

  assert(!isnan(y));
  if (y > _deactivateYPosThreshold && lambda <= _deactivateYVelThreshold)
    DEBUG_PRINT("MoreauJeanDirectProjectionOSI::removeInteractionInIndexSet DEACTIVATE.\n");
  return (y > _deactivateYPosThreshold && lambda <= _deactivateYVelThreshold);
}

#endif

#ifdef QFREE_ACTIVATION
bool MoreauJeanDirectProjectionOSI::addInteractionInIndexSet(SP::Interaction inter, unsigned int i)
{

  assert(i == 1);
  double y = (inter->y(i - 1))->getValue(0); // for i=1 y(i-1) is the position
#ifdef DEBUG_MESSAGES
  double yDot = (inter->y(i))->getValue(0); // for i=1 y(i) is the velocity
#endif
  DEBUG_PRINTF("MoreauJeanDirectProjectionOSI::addInteractionInIndexSet yref=%e, yDot=%e.\n", y, yDot);

  DEBUG_PRINTF("MoreauJeanDirectProjectionOSI::addInteractionInIndexSet  _activateYPosThreshold =%e, _activateYVelThreshold=%e\n",
               _activateYPosThreshold ,
               _activateYVelThreshold);

  assert(!isnan(y));

  if (y <= _activateYPosThreshold)
    DEBUG_PRINT("MoreauJeanDirectProjectionOSI::addInteractionInIndexSet ACTIVATE.\n");
  return (y <= _activateYPosThreshold);
}

bool MoreauJeanDirectProjectionOSI::removeInteractionInIndexSet(SP::Interaction inter, unsigned int i)

{
  assert(i == 1);
  double y = (inter->y(i - 1))->getValue(0); // for i=1 y(i-1) is the position
  double yDot = (inter->y(i))->getValue(0); // for i=1 y(i) is the velocity


  DEBUG_PRINTF("MoreauJeanDirectProjectionOSI::removeInteractionInIndexSet yref=%e, yDot=%e .\n", y, yDot);

  DEBUG_PRINTF("MoreauJeanDirectProjectionOSI::removeInteractionInIndexSet  _deactivateYPosThreshold =%e, _deactivateYVelThreshold=%e\n",
               _deactivateYPosThreshold ,
               _deactivateYVelThreshold);

  assert(!isnan(y));
  if (y > _deactivateYPosThreshold)
    DEBUG_PRINT("MoreauJeanDirectProjectionOSI::removeInteractionInIndexSet DEACTIVATE.\n");
  return (y > _deactivateYPosThreshold);
}

#endif