/* Siconos is a program dedicated to modeling, simulation and control
 * of non smooth dynamical systems.

 * Copyright 2016 INRIA.

 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at

 * http://www.apache.org/licenses/LICENSE-2.0

 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/
#include <stdio.h>
#include <stdlib.h>
#include "NonSmoothDrivers.h"
#include "frictionContact_test_function.h"


int main(void)
{
  int info = 0 ;

  char filename[50] = "./data/Confeti-ex13-Fc3D-SBM.dat";
  printf("Test on %s\n", filename);

  FILE * finput  =  fopen(filename, "r");
  SolverOptions * options = (SolverOptions *) malloc(sizeof(SolverOptions));
  info = fc3d_setDefaultSolverOptions(options, SICONOS_FRICTION_3D_VI_FPP);
  options->dparam[3] = -1e1; // optimal guessed value
  options->dparam[0] = 1e-8; //
  options->iparam[1] = 2; //
  options->iparam[2] = 0; //
  options->iparam[3] = 0; //

  info = frictionContact_test_function(finput, options);

  deleteSolverOptions(options);
  free(options);
  fclose(finput);
  printf("\nEnd of test on %s\n", filename);
  return info;
}
