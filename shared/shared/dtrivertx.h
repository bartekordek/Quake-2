#ifndef ____SHARED_SHARED_DTRIVERTX_H__
#define ____SHARED_SHARED_DTRIVERTX_H__

#include "shared/defines.h"

typedef struct
{
    byte v[3];  // scaled byte to fit in frame mins/maxs
    byte lightnormalindex;
} dtrivertx_t;


#endif // ____SHARED_SHARED_DTRIVERTX_H__