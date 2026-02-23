#ifndef __ref_gl_gl_image_h__
#define __ref_gl_gl_image_h__

#include "shared/boolean.h"
#include "shared/config.h"
#include "shared/defines.h"
#include "shared/image.h"

EXTERNC void	 Scrap_Upload (void);
EXTERNC qboolean scrap_dirty;
EXTERNC image_t *GL_LoadPic (const char *name, byte *pic, int width, int height, imagetype_t type, int bits);

#endif	// __ref_gl_gl_image_h__