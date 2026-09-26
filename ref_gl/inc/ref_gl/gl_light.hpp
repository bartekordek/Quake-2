#pragma once

#include "shared/config.h"
#include "math/vector.h"

typedef struct msurface_s msurface_t;
EXTERNC vec3_t			  lightspot;
EXTERNC void			  R_SetCacheState (msurface_t *surf);
