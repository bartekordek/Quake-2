#pragma once

#include "shared/config.h"
#include "shared/ref.h"

EXTERNC void Alias_ComputeMVP (const entity_t *e, int lefthand_mirror, float *out_mvp16);
EXTERNC void Alias_DrawTriangles (const float *interleaved9, int vertex_count, unsigned int skin_texnum, const float *mvp16,
								  int use_texture);

/* Sky quad in camera-relative space. verts: 4 * (x,y,z,s,t). */
EXTERNC void R_DrawSkyQuad (unsigned int texnum, float rotate_deg, const float axis[3], const float origin[3],
							const float verts[4][5]);

/* Camera-space triangles, pos3+uv2+rgba4. Used for particles. */
EXTERNC void R_DrawCameraTriangles (const float *interleaved9, int vertex_count, unsigned int texnum, int blend, int depth_write);
