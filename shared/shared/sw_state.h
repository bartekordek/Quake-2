#ifndef ____SHARED_SHARED_SW_STATE_H__
#define ____SHARED_SHARED_SW_STATE_H__

#ifndef ____SHARED_SW_STATE_H__
#define ____SHARED_SW_STATE_H__

#include "shared/defines.h"

typedef struct swstate_s
{
    qboolean fullscreen;
    int prev_mode;  // last valid SW mode

    byte gammatable[256];
    byte currentpalette[1024];

} swstate_t;

#endif  // ____SHARED_SW_STATE_H__

#endif // ____SHARED_SHARED_SW_STATE_H__