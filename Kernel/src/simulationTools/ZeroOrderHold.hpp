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
/*! \file
  ZeroOrderHold Time-Integrator for Dynamical Systems
  */

#ifndef ZEROORDERHOLD_H
#define ZEROORDERHOLD_H

#include "OneStepIntegrator.hpp"
#include "SimpleMatrix.hpp"

class Simulation;
class SiconosMatrix;

const unsigned int ZOHSTEPSINMEMORY = 1;

/**  ZeroOrderHold Time-Integrator for Dynamical Systems
 *
 *  \author SICONOS Development Team - copyright INRIA
 *  \version 3.0.0.
 *  \date (Creation) Apr 26, 2004
 *
 * See User's guide, \ref docSimuZeroOrderHoldTS for details.
 *
 * ZeroOrderHold class is used to define some time-integrators methods for a
 * list of dynamical systems.

 * A ZeroOrderHold instance is defined by the value of theta and the list of
 * concerned dynamical systems.  Each DynamicalSystem is associated to
 * a SiconosMatrix, named "W"
 *
 * W matrices are initialized and computed in initW and
 * computeW. Depending on the DS type, they may depend on time and DS
 * state (x).
 *
 * For first order systems, the implementation uses _r for storing the
 * the input due to the nonsmooth law. This ZeroOrderHold scheme assumes that the
 * relative degree is zero or one and one level for _r is sufficient
 *
 * For Lagrangian systems, the implementation uses _p[1] for storing the
 * discrete impulse.
 *
 * Main functions:
 *
 * - computeFreeState(): computes xfree (or vfree), dynamical systems
 *   state without taking non-smooth part into account \n
 *
 * - updateState(): computes x (q,v), the complete dynamical systems
 *    states.
 *
 */
class ZeroOrderHold : public OneStepIntegrator
{
private:
  /** default constructor
   */
  ZeroOrderHold() {};

protected:
  /** serialization hooks
  */
  ACCEPT_SERIALIZATION(ZeroOrderHold);

  /** nslaw effects */
  struct _NSLEffectOnFreeOutput;
  friend struct _NSLEffectOnFreeOutput;

  /** Unused for now */
  bool _useGammaForRelation;

  /** init Phi[ds], ZeroOrderHold matrix, at time t
   *  \param t the time
   *  \param ds a pointer to DynamicalSystem
   */
//  void initMatrixAd(const double t, SP::DynamicalSystem ds);

  /** init Ad[ds] and possibly Bd[ds] or Ld[ds] , ZeroOrderHold matrices, at time t
   *  \param t the time
   *  \param ds a pointer to DynamicalSystem
   *  \param inter the control interaction
   */
//  void initMatrices(const double t, SP::DynamicalSystem ds, const Interaction& inter);

  /** Initialize the integrators
   * \param ds the DynamicalSystem
   * \param withInteraction true if we have an Interaction
   */
//  void initIntegrators(const DynamicalSystem& ds, const bool withInteraction);

  /** compute Psi[ds], ZeroOrderHold matrix, at time t
   *  \param ds the DynamicalSystem
   *  \param rel the relation
   */
//  void computeBd(const DynamicalSystem& ds, const Relation& rel);

  /** compute Psi[ds], ZeroOrderHold matrix, if the DS is TI
   *  \param ds the DynamicalSystem
   *  \param rel the relation
   */
//  void computeBdTI(const DynamicalSystem& ds, const Relation& rel);

  /** compute Psi[ds], ZeroOrderHold matrix, if the DS is TI
   *  \param ds the DynamicalSystem
   */
//  void computeLdTI(const DynamicalSystem& ds);

  /** compute Phi[ds], ZeroOrderHold matrix, at time t
   *  \param ds the DynamicalSystem
   */
//  void computeAd(const DynamicalSystem& ds);

  /** compute AdInt[ds], ZeroOrderHold matrix, at time t
   *  \param ds the DynamicalSystem
   */
//  void computeAdInt(const DynamicalSystem& ds);

public:

  /** constructor from xml file
   *  \param osiXML the corresponding XML object
   *  \param dsList the set of all DS in the NSDS
   */
  ZeroOrderHold(SP::OneStepIntegratorXML osiXML, SP::DynamicalSystemsSet dsList);

  /** constructor from a minimum set of data: one DS
   *  \param ds the DynamicalSystem linked to the OneStepIntegrator
   */
  ZeroOrderHold(SP::DynamicalSystem ds);

  /** destructor
  */
  virtual ~ZeroOrderHold() {};

  // --- GETTERS/SETTERS ---

  /** get \f$\Phi\f$ corresponding to DynamicalSystem ds
   * \param ds the DynamicalSystem
   * \return pointer to a SiconosMatrix
   */
  const SiconosMatrix& Ad(SP::DynamicalSystem ds);

  /** set the value of Phi[ds] to nePhi
   * \param newPhi the new matrix
   * \param ds a pointer to DynamicalSystem
   */
//  void setPhi(const SiconosMatrix& newPhi, SP::DynamicalSystem ds);

  /** set Psi[ds] to pointer newPtr
   * \param newPtr pointer to the new matrix
   * \param ds a pointer to DynamicalSystem
   */
//  void setPhiPtr(SP::SimpleMatrix newPtr, SP::DynamicalSystem ds);

  /** get \f$B_d\f$ corresponding to DynamicalSystem ds
   * \param ds the DynamicalSystem
   * \return pointer to a SiconosMatrix
   */
  const SiconosMatrix& Bd(SP::DynamicalSystem ds);

  /** get \f$\int_{t_k}^{t_{k+1}}\!\Phi(t_{k+1}, \tau)\mathrm{d}\tau\f$ corresponding to DynamicalSystem ds
   * \param ds the DynamicalSystem
   * \return pointer to a SiconosMatrix
   */
//  inline SP::SimpleMatrix intPhi(SP::DynamicalSystem ds)
//  {
//    assert(_AdIntMap[ds.number()] &&
//        "ZeroOrderHold::intPhi(ds): _AdIntMap[ds.number()] == NULL.");
//    return _AdIntMap[ds.number()];
//  };
  // --- OTHER FUNCTIONS ---

  /** initialization of the ZeroOrderHold integrator */
  void initialize();

  /** return the maximum of all norms for the "ZeroOrderHold-discretized" residus of DS
    \return a double
    */
  double computeResidu();

  /** Perform the integration of the dynamical systems linked to this integrator
   *  without taking into account the nonsmooth input (_r or _p)
   */
  virtual void computeFreeState();

  /** Compute the Output (y) which corresponds to the free state (state without
      taking into account the nonsmooth input) plus the possible contribution of
      the nslaw
      \param vertex of the interaction graph
      \param osnsp a pointer to the OneStepNSProblem
   */
  virtual void computeFreeOutput(InteractionsGraph::VDescriptor& vertex_inter, OneStepNSProblem * osnsp);

  /** Apply the rule to one Interaction to known if is it should be included
   * in the IndexSet of level i
   * \param inter a pointer to the Interaction to be added
   * \param i the level of the IndexSet
   * \return true if y<=0
   */
  virtual bool addInteractionInIndexSet(SP::Interaction inter, unsigned int i);

  /** Apply the rule to one Interaction to known if is it should be removed
   * in the IndexSet of level i
   * \param inter a pointer to the Interaction to be removed
   * \param i the level of the IndexSet
   * \return true if y>0
   */
  virtual bool removeInteractionInIndexSet(SP::Interaction inter, unsigned int i);


  /** Unused
   * \param time current time
   */
  void prepareNewtonIteration(double time);


  /** integrate the system, between tinit and tend (->iout=true), with possible stop at tout (->iout=false)
   *  \param tinit initial time
   *  \param tend end time
   *  \param tout real end time
   *  \param notUsed useless flag (for ZeroOrderHold, used in Lsodar)
   */
  void integrate(double& tinit, double& tend, double& tout, int& notUsed);

  /** updates the state of the Dynamical Systems
   *  \param level  level of interest for the dynamics: not used at this moment
   */
  virtual void updateState(const unsigned int level);

  /** Displays the data of the ZeroOrderHold's integrator
  */
  void display();

  /** insert a dynamical system in this Integrator
   *  \param ds a SP::DynamicalSystem to insert
   */
  void insertDynamicalSystem(SP::DynamicalSystem ds);

  void updateMatrices(SP::DynamicalSystem ds);

  /** visitors hook
  */
  ACCEPT_STD_VISITORS();

};

//TYPEDEF_SPTR(ZeroOrderHold)
#endif // ZEROORDERHOLD_H
