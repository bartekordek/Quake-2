#ifndef __ref_gl_gl_warp_h__
#define __ref_gl_gl_warp_h__

#include "math/vector.h"

EXTERNC void  R_SetSky (const char *name, float rotate, vec3_t axis);
EXTERNC float r_turbsin[256];

#endif	// __ref_gl_gl_warp_h__