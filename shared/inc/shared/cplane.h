#ifndef __shared_cplane_h__
#define __shared_cplane_h__

#include "shared/defines.h"
#include "math/vector.h"

// plane_t structure
// !!! if this is changed, it must be changed in asm code too !!!
typedef struct cplane_s
{
	vec3_t normal;
	float  dist;
	byte   type;	  // for fast side tests
	byte   signbits;  // signx + (signy<<1) + (signz<<1)
	byte   pad[2];
} cplane_t;

#endif // __shared_cplane_h__