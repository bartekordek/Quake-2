#ifndef ____SHARED_MSURFACE_H__
#define ____SHARED_MSURFACE_H__

#include "shared/plane.h"
#include "shared/plane.h"
#include "shared/glpoly.h"
#include "shared/mtexinfo.h"

typedef struct msurface_s
{
    int visframe;// should be drawn when node is crossed
    int dlightframe;
    int dlightbits;

    plane_t *plane;
    int flags;

    int firstedge;// look up in model->surfedges[], negative numbers
    int numedges;// are backwards edges

// surface generation data
    struct surfcache_s *cachespots[MIPLEVELS];

    short texturemins[2];
    short extents[2];
    mtexinfo_t *texinfo;

    int light_s, light_t;// gl lightmap coordinates
    int dlight_s, dlight_t;// gl lightmap coordinates for dynamic lightmaps

    glpoly_t *polys;// multiple if warped
    struct msurface_s *texturechain;
    struct  msurface_s *lightmapchain;

// lighting info
    int lightmaptexturenum;
    byte styles[MAXLIGHTMAPS];
    float cached_light[MAXLIGHTMAPS];    // values currently used in lightmap
    byte *samples;// [numstyles*surfsize]
    struct msurface_s *nextalphasurface;
} msurface_t;

#endif // ____SHARED_MSURFACE_H__