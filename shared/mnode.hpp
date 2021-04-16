#ifndef ____SHARED_MNODE_H__
#define ____SHARED_MNODE_H__

#include "shared/plane.hpp"

typedef struct mnode_s
{
// common with leaf
    int            contents;        // CONTENTS_NODE, to differentiate from leafs
    int            visframe;        // node needs to be traversed if current

    short        minmaxs[6];        // for bounding box culling

    struct mnode_s* parent;

// node specific
    plane_t* plane;
    struct mnode_s* children[2];

    unsigned short        firstsurface;
    unsigned short        numsurfaces;
} mnode_t;


#endif // ____SHARED_MNODE_H__