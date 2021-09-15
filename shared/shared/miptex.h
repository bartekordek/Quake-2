#ifndef ____SHARED_SHARED_MIPTEX_H__
#define ____SHARED_SHARED_MIPTEX_H__

#include "shared/defines.h"

/*
==============================================================================

  .WAL texture file format

==============================================================================
*/

#define MIPLEVELS 4
typedef struct miptex_s
{
    char name[32];
    unsigned width, height;
    unsigned offsets[MIPLEVELS];  // four mip maps stored
    char animname[32];            // next frame in animation chain
    int flags;
    int contents;
    int value;
} miptex_t;

#endif // ____SHARED_SHARED_MIPTEX_H__