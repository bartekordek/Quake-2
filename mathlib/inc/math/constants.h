#ifndef __math_constants_h__
#define __math_constants_h__

#include "math/config.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288	// matches value in gcc v2 math.h
#endif

#ifndef M_PI_F
#define M_PI_F 3.14159265358979323846264338327950288f  // matches value in gcc v2 math.h
#endif									 // M_PI_F

EXTERNC  float  g_pi_f;
EXTERNC  double g_pi_d;

#endif	// __math_constants_h__
