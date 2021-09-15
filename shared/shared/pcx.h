#ifndef ____SHARED_SHARED_PCX_H__
#define ____SHARED_SHARED_PCX_H__

/*
========================================================================

PCX files are used for as many images as possible

========================================================================
*/

typedef struct
{
    char manufacturer;
    char version;
    char encoding;
    char bits_per_pixel;
    unsigned short xmin, ymin, xmax, ymax;
    unsigned short hres, vres;
    unsigned char palette[48];
    char reserved;
    char color_planes;
    unsigned short bytes_per_line;
    unsigned short palette_type;
    char filler[58];
    unsigned char data;  // unbounded
} pcx_t;

#endif // ____SHARED_SHARED_PCX_H__