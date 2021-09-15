#ifndef ____SHARED_SHARED_CPLANE_H__
#define ____SHARED_SHARED_CPLANE_H__

#include "shared/defines.h"

// plane_t structure
// !!! if this is changed, it must be changed in asm code too !!!
typedef struct cplane_s
{
    vec3_t normal;
    float dist;
    byte type;      // for fast side tests
    byte signbits;  // signx + (signy<<1) + (signz<<1)
    byte pad[2];
} cplane_t;


#endif // ____SHARED_SHARED_CPLANE_H__