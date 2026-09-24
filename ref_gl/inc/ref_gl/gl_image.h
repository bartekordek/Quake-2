#ifndef __ref_gl_gl_image_h__
#define __ref_gl_gl_image_h__

#include "shared/config.h"
#include "shared/boolean.h"
#include "shared/image.h"

extern void		Scrap_Upload (void);
extern qboolean scrap_dirty;
EXTERNC void	GL_FreeUnusedImages (void);
EXTERNC void	GL_TextureMode (const char *string);
EXTERNC void	GL_TextureAlphaMode (const char *string);
EXTERNC void	GL_TextureSolidMode (const char *string);
EXTERNC image_t *GL_LoadPic (const char *name, byte *pic, int width, int height, imagetype_t type, int bits);

#endif	// __ref_gl_gl_image_h__