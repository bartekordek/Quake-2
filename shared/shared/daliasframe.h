#ifndef ____SHARED_SHARED_DALIASFRAME_H__
#define ____SHARED_SHARED_DALIASFRAME_H__

#include "shared/dtrivertx.h"

typedef struct
{
    float scale[3];        // multiply byte verts by this
    float translate[3];    // then add this
    char name[16];         // frame name from grabbing
    dtrivertx_t verts[1];  // variable sized
} daliasframe_t;

#endif // ____SHARED_SHARED_DALIASFRAME_H__