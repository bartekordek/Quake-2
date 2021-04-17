#ifndef ____SHARED_PLANE_H__
#define ____SHARED_PLANE_H__

#include "shared/defines.hpp"

// plane_s structure
// !!! if this is changed, it must be changed in asm code too !!!
struct plane_s
{
    vec3_t normal;
    float dist;
    byte type;// for fast side tests
    byte signbits;// signx + (signy<<1) + (signz<<1)
    byte pad[2];
};

#endif // ____SHARED_PLANE_H__