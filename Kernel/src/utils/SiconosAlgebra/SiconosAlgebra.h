/* Siconos-Kernel version 2.0.1, Copyright INRIA 2005-2006.
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
 * Contact: Vincent ACARY vincent.acary@inrialpes.fr
 */


/** ! \file SiconosAlgebra.h
    \brief Header file for Siconos Algebra objects

    This file provides typedef for matrix and vector objects, const values and so on ...
    \author SICONOS Development Team - copyright INRIA
    \date (creation) 10/17/2006
*/

#ifndef SiconosAlgebra
#define SiconosAlgebra

#include "SiconosConst.h"
#include <deque>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_sparse.hpp>
#include <boost/numeric/ublas/triangular.hpp>
#include <boost/numeric/ublas/symmetric.hpp>
#include <boost/numeric/ublas/banded.hpp>

namespace ublas = boost::numeric::ublas;

/** type of object used to save indices */
typedef std::vector<unsigned int> Index;

/** Various matrix types available in Siconos **/

/** DenseMat is a typedef of boost::ublas::numeric::matrix<double, column_major, std::vector<double> >
 */
typedef ublas::matrix<double, ublas::column_major, std::vector<double> > DenseMat;

/** TriangMat is a typedef of boost::ublas::numeric::triangular_matrix<double, upper, column_major, std::vector<double> >
 */
typedef ublas::triangular_matrix<double, ublas::upper, ublas::column_major, std::vector<double> > TriangMat;

/** SymMat is a typedef of boost::ublas::numeric::symmetric_matrix<double, upper, column_major, std::vector<double> >
 */
typedef ublas::symmetric_matrix<double, ublas::upper, ublas::column_major, std::vector<double> > SymMat;

/** BandedMat is a typedef of boost::ublas::numeric::banded_matrix<double, column_major, std::vector<double> >
 */
typedef ublas::banded_matrix<double, ublas::column_major, std::vector<double> > BandedMat;

/** SparseMat is a typedef of boost::ublas::numeric::mapped_matrix<double>
 */
typedef ublas::compressed_matrix<double, ublas::column_major, 0, Index, std::vector<double> > SparseMat;

/** Various vector types available in Siconos **/

/** DenseVect is a typedef of boost::ublas::numeric::vector<double, std::vector<double> >
 */
typedef ublas::vector<double, std::vector<double> > DenseVect;

/** SparseVect is a typedef of boost::ublas::numeric::mapped<double>
 */
typedef ublas::compressed_vector<double, 0, Index, std::vector<double> > SparseVect;

/** Const from old version of SiconosVector - To be reviewed */
const char N_DOUBLE_PRECISION[] = "%1.52e "; // double mantisse precision /!\ DEPENDS ON MACHINE
const unsigned int M_MAXSIZEFORDISPLAY = 10;
const std::string DEFAULT_FORMAT = "ascii";

/** value used to compare matrices. Matrices A and B are equal when (A-B).normInf()<tolerance. */
const double tolerance = 1e-14;

/** TYP is an enumerated type of DENSE, TRIANGULAR, SYMMETRIC, SPARSE, BANDED. TYP is used to describe the type of matrix or vector we want to construct.
 */
enum TYP {DENSE = 1, TRIANGULAR, SYMMETRIC, SPARSE, BANDED};

// Notes:
// Vector definition in boost: vector<T,A> see http://www.boost.org/libs/numeric/ublas/doc/vector.htm
// T: value type
// A: storage type

/** Objects used to define block matrices and vectors:*/

class SiconosMatrix;
/** A collection of pointers to matrices ; blocksMat is a typedef of boost::ublas::numeric::mapped_matrix<SiconosMatrix* > */
typedef ublas::compressed_matrix<SiconosMatrix*> BlocksMat;

/**  iterator for BlocksMat*/
typedef BlocksMat::iterator1 BlockIterator1;
/**  iterator for BlocksMat*/
typedef BlocksMat::iterator2 BlockIterator2;
/**  iterator for BlocksMat*/
typedef BlocksMat::const_iterator1 ConstBlockIterator1;
/**  iterator for BlocksMat*/
typedef BlocksMat::const_iterator2 ConstBlockIterator2;

class SiconosVector;
/** A collection of pointers to vectors;  blocksVect is a typedef of boost::ublas::numeric::mapped_matrix<SiconosMatrix* > */
typedef std::vector<SiconosVector*> BlocksVect;
/** iterators for BlocksVect*/
typedef BlocksVect::iterator BlockVectIterator;
/** Const iterators for BlocksVect*/
typedef BlocksVect::const_iterator ConstBlockVectIterator;
#endif
/** Some containers for vectors - Used for example to handle x and its derivatives in DynamicalSystem. */
typedef std::vector<SiconosVector*> VectorOfVectors;

/** Iterator through vector of vectors */
typedef VectorOfVectors::iterator VectorOfVectorsIterator;

/** Type used for inside-class allocation checking */
typedef std::map<std::string, bool> AllocationFlagsMap;

/** Type used for inside-class allocation checking */
typedef std::deque<bool>  AllocationFlags;

/** Some containers for matrices - Used for example to handle the various jacobian in LagrangianDS. */
typedef std::vector<SiconosMatrix*> VectorOfMatrices;

/** Iterator through vector of matrices */
typedef VectorOfMatrices::iterator VectorOfMatricesIterator;
