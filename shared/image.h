#ifndef ____SHARED_IMAGE_H__
#define ____SHARED_IMAGE_H__

#include "shared/defines.h"
#include "shared/enum_image_type.h"

typedef struct image_s
{
    char name[MAX_QPATH];// game path, including extension
    imagetype_t type;
    int width, height;// source image

    int registration_sequence;// 0 = free
    qboolean transparent;// true if any 255 pixels in image
    byte *pixels[4];// mip levels
    int upload_width, upload_height;// after power of two and picmip
    struct msurface_s *texturechain;    // for sort-by-texture world drawing
    int texnum;// gl texture binding
    float sl, tl, sh, th;// 0,0 - 1,1 unless part of the scrap
    qboolean scrap;
    qboolean has_alpha;

    qboolean paletted;
} image_t;

#endif // ____SHARED_IMAGE_H__