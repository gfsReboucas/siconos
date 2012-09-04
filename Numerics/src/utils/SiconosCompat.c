/* Siconos-Numerics, Copyright INRIA 2005-2011.
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


#include <stdlib.h>
#include <math.h>

#ifdef _MSC_VER

float _sqrtf(float x)
{
  return sqrtf(x);
}
float _logf(float x)
{
  return logf(x);
}
extern "C" long int lroundf(float x)
{
  return (long int)floorl(x + .5);
}
extern "C" double __cdecl __powidf2(double a, int b)
{
  const int recip = b < 0;
  double r = 1;
  while (1)
  {
    if (b & 1)
      r *= a;
    b /= 2;
    if (b == 0)
      break;
    a *= a;
  }
  return recip ? 1 / r : r;
}

#endif

#ifdef _MSC_VER

#include <float.h>

// Classify floating point number - usually defined in math.h
extern "C" int __fpclassify(double x)
{
  return _fpclass(x);
}

#endif