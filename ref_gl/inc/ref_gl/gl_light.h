#ifndef __ref_gl_gl_light_h__
#define __ref_gl_gl_light_h__

#include "shared/config.h"
#include "math/vector.h"

typedef struct msurface_s msurface_t;
EXTERNC vec3_t			  lightspot;
EXTERNC void			  R_SetCacheState (msurface_t *surf);

#endif	// __ref_gl_gl_light_h__