#ifndef __ref_gl_gl_light_h__
#define __ref_gl_gl_light_h__

#include "ref_gl/gl_model.h"

EXTERNC void R_SetCacheState (msurface_t *surf);
EXTERNC void R_BuildLightMap (msurface_t *surf, byte *dest, int stride);

#endif	// __ref_gl_gl_light_h__