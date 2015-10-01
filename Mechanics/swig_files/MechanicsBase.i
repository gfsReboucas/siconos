// -*- c++ -*-
// Base SWIG interface for siconos mechanics

%include start.i
// generated docstrings from doxygen xml output
%include mechanics-docstrings.i

%include path.i

#undef WITH_IO
#undef WITH_SERIALIZATION

#ifdef WITH_IO
%{
#include <SiconosFull.hpp>
%}
#endif

%include sharedPointers.i

%{
#include <MechanicsFwd.hpp>
%}
%include <MechanicsFwd.hpp>

%include picklable.i

%include path.i

%include handleException.i
%include stl.i
%include KernelTypes.i

%{
#include <SiconosKernel.hpp>
#include <boost/typeof/typeof.hpp>
%}

%import kernel.i

%include pyRegister.i

 // force the definition of SWIGTYPE_p_Interaction...
typedef Interaction Interaction;
