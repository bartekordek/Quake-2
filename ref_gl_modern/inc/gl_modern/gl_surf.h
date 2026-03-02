#ifndef __ref_gl_gl_surf_h__
#define __ref_gl_gl_surf_h__

#include "gl_modern/gl_model.h"

EXTERNC void	 GL_BuildPolygonFromSurface (msurface_t *fa);
EXTERNC void	 GL_CreateSurfaceLightmap (msurface_t *surf);
EXTERNC image_t *R_TextureAnimation (mtexinfo_t *tex);

#endif	// __ref_gl_gl_surf_h__