#ifndef ____SHARED_SHARED_GLSTATE_H__
#define ____SHARED_SHARED_GLSTATE_H__

#ifndef ____SHARED_GLSTATE_H__
#define ____SHARED_GLSTATE_H__

#include "shared/defines.h"

typedef struct
{
    float inverse_intensity;
    qboolean fullscreen;

    int prev_mode;

    unsigned char* d_16to8table;

    int lightmap_textures;

    int currenttextures[2];
    int currenttmu;

    float camera_separation;
    qboolean stereo_enabled;

    unsigned char originalRedGammaTable[256];
    unsigned char originalGreenGammaTable[256];
    unsigned char originalBlueGammaTable[256];
} glstate_t;

#endif // ____SHARED_GLSTATE_H__

#endif // ____SHARED_SHARED_GLSTATE_H__