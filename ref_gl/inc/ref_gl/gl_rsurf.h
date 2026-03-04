#ifndef __ref_gl_gl_rsurf_h__
#define __ref_gl_gl_rsurf_h__

#include "ref_gl/gl_model.h"
#include "shared/ref.h"

EXTERNC void GL_BuildPolygonFromSurface (msurface_t *fa);
EXTERNC void GL_CreateSurfaceLightmap (msurface_t *surf);
EXTERNC void GL_BeginBuildingLightmaps (model_t *m);
EXTERNC void GL_EndBuildingLightmaps (void);
EXTERNC void R_DrawBrushModel (entity_t *e);
EXTERNC void R_DrawAlphaSurfaces (void);
EXTERNC void R_MarkLeaves (void);
EXTERNC void R_DrawWorld (void);

#endif	// __ref_gl_gl_rsurf_h__