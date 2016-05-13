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
#ifndef LCP_PROBLEM_C
#define LCP_PROBLEM_C


#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>

#include "LinearComplementarityProblem.h"
#include "misc.h"

void linearComplementarity_display(LinearComplementarityProblem* problem)
{

  assert(problem);
  int i, n = problem->size;
  printf("LinearComplementarityProblem Display :\n-------------\n");
  printf("size :%d \n", problem->size);
  if (problem->M)
  {
    printf("M matrix:\n");
    display(problem->M);
  }
  else
    printf("No M matrix:\n");

  if (problem->q)
  {
    printf("q vector:\n");
    for (i = 0; i < n; i++) printf("q[ %i ] = %12.8e\n", i, problem->q[i]);
  }
  else
    printf("No q vector:\n");

}

int linearComplementarity_printInFile(LinearComplementarityProblem*  problem, FILE* file)
{
  if (! problem)
  {
    fprintf(stderr, "Numerics, LinearComplementarityProblem printInFile failed, NULL input.\n");
    exit(EXIT_FAILURE);
  }
  int i;
  int n = problem->size;
  fprintf(file, "%d\n", n);
  printInFile(problem->M, file);
  for (i = 0; i < problem->M->size1; i++)
  {
    fprintf(file, "%32.24e ", problem->q[i]);
  }
  return 1;
}

int linearComplementarity_newFromFile(LinearComplementarityProblem* problem, FILE* file)
{
  int n = 0;
  int i;

  CHECK_IO(fscanf(file, "%d\n", &n));
  problem->size = n;
  problem->M = newNumericsMatrix();

  newFromFile(problem->M, file);

  problem->q = (double *) malloc(problem->M->size1 * sizeof(double));
  for (i = 0; i < problem->M->size1; i++)
  {
    CHECK_IO(fscanf(file, "%lf ", &(problem->q[i])));
  }
  return 1;
}
int linearComplementarity_newFromFilename(LinearComplementarityProblem* problem, char* filename)
{
  int info = 0;
  FILE * file = fopen(filename, "r");

  info = linearComplementarity_newFromFile(problem, file);

  fclose(file);
  return info;
}

void freeLinearComplementarityProblem(LinearComplementarityProblem* problem)
{
  freeNumericsMatrix(problem->M);
  free(problem->M);
  free(problem->q);
  free(problem);
  problem = NULL;
}



#endif

