#ifndef ____SHARED_MLEAF_H__
#define ____SHARED_MLEAF_H__

#include "shared/msurface.hpp"

typedef struct mleaf_s
{
// common with node
    int            contents;        // wil be a negative contents number
    int            visframe;        // node needs to be traversed if current

    float        minmaxs[6];        // for bounding box culling

    struct mnode_s* parent;

// leaf specific
    int            cluster;
    int            area;

    msurface_t* *firstmarksurface;
    int            nummarksurfaces;
    int            key;            // BSP sequence number for leaf's contents
} mleaf_t;

#endif // ____SHARED_MLEAF_H__