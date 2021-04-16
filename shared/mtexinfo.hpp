#ifndef ____SHARED_MTEXINFO_H__
#define ____SHARED_MTEXINFO_H__

#include "shared/image.hpp"

typedef struct mtexinfo_s
{
    float        vecs[2][4];
    float        mipadjust;

    int            flags;
    int            numframes;
    struct mtexinfo_s    *next;        // animation chain
    image_t        *image;
} mtexinfo_t;

#endif // ____SHARED_MTEXINFO_H__