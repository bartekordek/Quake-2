#ifndef ____SHARED_DMODEL_H__
#define ____SHARED_DMODEL_H__

#include "shared/defines.hpp"

//
// in memory representation
//
// !!! if this is changed, it must be changed in asm_draw.h too !!!

typedef struct
{
    vec3_t        mins, maxs;
    vec3_t        origin;        // for sounds or lights
    float        radius;
    int            headnode;
    int            visleafs;        // not including the solid leaf 0
    int            firstface, numfaces;
} dmodel_t;

#endif // ____SHARED_DMODEL_H__