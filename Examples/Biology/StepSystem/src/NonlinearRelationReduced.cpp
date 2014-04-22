#ifndef NONLINEARRELATIONREDUCED_CPP
#define NONLINEARRELATIONREDUCED_CPP

#include "NonlinearRelationReduced.h"

//#include "const.h"
#define SICONOS_DEBUG

NonlinearRelationReduced::NonlinearRelationReduced():
  FirstOrderType2R()
{
}

void NonlinearRelationReduced::initComponents(Interaction& inter, VectorOfBlockVectors& DSlink, VectorOfVectors& workV, VectorOfSMatrices& workM)
{
  FirstOrderType2R::initComponents(inter, DSlink, workV, workM);
  unsigned int sizeY = inter.getSizeOfY();
  unsigned int sizeDS = inter.getSizeOfDS();

  SiconosVector& lambda = *inter.lambda(0);

  SiconosVector& x = *workV[FirstOrderRVec::x];
  x = *DSlink[FirstOrderRDS::x];

  double t0 = 0;

  computeh(t0, x, lambda, *inter.Halpha());
  computeg(t0, lambda, *workV[FirstOrderRVec::g_alpha]);
  *DSlink[FirstOrderRDS::r] = *workV[FirstOrderRVec::g_alpha];
  computeJach(t0, inter, DSlink, workV, workM);
  computeJacg(t0, inter, DSlink, workV, workM);


}

/*y = h(X)*/
void NonlinearRelationReduced::computeh(double t, SiconosVector& x, SiconosVector& lambda, SiconosVector& y)
{

  y(0) =  8.0 - x(1);

}



  /** default function to compute jacobianH
   *  \param double : current time
   *  \param index for jacobian (0: jacobian according to x, 1 according to lambda)
   */

  /** default function to compute jacobianG according to lambda
   *  \param double : current time
   *  \param index for jacobian: at the time only one possible jacobian => i = 0 is the default value .
   */


/*g=g(lambda)*/
void NonlinearRelationReduced::computeg(double t, SiconosVector& lambda, SiconosVector& r)
{
  r(1) = 40.0 * (1 - lambda(0));
  r(0) =  0.0;
}

void NonlinearRelationReduced::computeJachlambda(double t, SiconosVector& x, SiconosVector& lambda, SimpleMatrix& D)
{
  D.zero();
}

void NonlinearRelationReduced::computeJachx(double t, SiconosVector& x, SiconosVector& lambda, SimpleMatrix& C)
{

  C(0, 0) =  0;
  C(0, 1) = -1;

}

void NonlinearRelationReduced::computeJacglambda(double t, SiconosVector& lambda, SimpleMatrix& B)
{
  B(0, 0) =  0.0;
  B(1, 0) = -40.0;
}
#endif

