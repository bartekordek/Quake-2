#ifndef ____SHARED_VIDDEF_H__
#define ____SHARED_VIDDEF_H__

typedef unsigned char pixel_t;

struct viddef_t
{
    pixel_t* buffer;    // invisible buffer
    pixel_t* colormap;  // 256 * VID_GRADES size
    pixel_t* alphamap;  // 256 * 256 translucency map
    int rowbytes;       // may be > width if displayed in a window
                        // can be negative for stupid dibs
    int width;
    int height;
};

#endif  // ____SHARED_VIDDEF_H__