#ifndef SICONOSCONFIG_H
#define SICONOSCONFIG_H

#define WITH_CMAKE
#cmakedefine HAVE_SICONOS_KERNEL
#cmakedefine HAVE_SICONOS_IO
#cmakedefine HAVE_SICONOS_MECHANICS
#cmakedefine HAVE_SICONOS_CONTROL
#cmakedefine HAVE_PATHFERRIS
#cmakedefine HAVE_MLCPSIMPLEX
#cmakedefine HAVE_TIME_H
#cmakedefine HAVE_SYSTIMES_H
#cmakedefine HAVE_MPI
#cmakedefine WITH_MUMPS
#cmakedefine WITH_TIMERS
#cmakedefine DUMP_PROBLEM
#cmakedefine WITH_FCLIB
#cmakedefine BUILD_AS_CPP
#cmakedefine WITH_LPSOLVE
#cmakedefine HAS_EXTREME_POINT_ALGO
#cmakedefine HAVE_VTK
#cmakedefine HAVE_BULLET
#cmakedefine HAVE_OCC
#cmakedefine HAVE_SERIALIZATION
#cmakedefine HAVE_GENERATION

// Is cblas available? 
#cmakedefine HAS_CBLAS

// Where does it comes from? 
#cmakedefine HAS_MKL_CBLAS
#cmakedefine HAS_ACCELERATE // includes also lapack from Accelerate
#cmakedefine HAS_ATLAS_CBLAS 
#cmakedefine HAS_OpenBLAS // it *MAY* also includes lapacke or lapack from netlib
#cmakedefine HAS_GenericCBLAS

// Which Lapack? 
#cmakedefine HAS_MKL_LAPACKE
#cmakedefine HAS_ATLAS_LAPACK
#cmakedefine HAS_LAPACKE // lapacke.h has been found
#cmakedefine HAS_CLAPACK  // clapack.h has been found
#cmakedefine HAS_OpenBLAS_LAPACK

// Which functions are defined in lapack? 
#cmakedefine HAS_LAPACK_DGESVD
#cmakedefine HAS_LAPACK_DTRTRS
#cmakedefine HAS_LAPACK_DGELS

// Some definitions required for boost numeric_bindings
#if defined(HAS_CBLAS)
#define BOOST_NUMERIC_BINDINGS_BLAS_CBLAS
#endif

#if defined(HAS_MKL_CBLAS)
#define BOOST_NUMERIC_BINDINGS_BLAS_MKL
#endif

// Gams stuff
#cmakedefine GAMS_MODELS_SOURCE_DIR "@GAMS_MODELS_SOURCE_DIR@"
#cmakedefine GAMS_MODELS_SHARE_DIR "@GAMS_MODELS_SHARE_DIR@"
#cmakedefine GAMS_DIR "@GAMS_DIR@"
#cmakedefine HAVE_GAMS_C_API

// Which version of C++ was used to compile siconos, needed for swig
#define SICONOS_CXXVERSION @CXXVERSION@


#endif