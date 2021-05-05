#ifndef ____REF_GL_GL_MODEL_H__
#define ____REF_GL_GL_MODEL_H__

/*
Copyright (C) 1997-2001 Id Software, Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

/*

d*_t structures are on-disk representations
m*_t structures are in-memory

*/

/*
==============================================================================

BRUSH MODELS

==============================================================================
*/

#include "shared/mtexinfo.hpp"
#include "shared/msurface.hpp"
#include "shared/mnode.hpp"
#include "shared/mleaf.hpp"
#include "shared/model.hpp"

#define SIDE_FRONT    0
#define SIDE_BACK    1
#define SIDE_ON        2


#define SURF_PLANEBACK        2
#define SURF_DRAWSKY        4
#define SURF_DRAWTURB        0x10
#define SURF_DRAWBACKGROUND    0x40
#define SURF_UNDERWATER        0x80


//============================================================================

void ref_gl_ModInit (void);
void ref_gl_ModClearAll (void);
model_t *ref_gl_ModForName (char *name, bool crash);
mleaf_t *ref_gl_ModPointInLeaf (float *p, model_t *model);
byte    *ref_gl_ModClusterPVS (int cluster, model_t *model);

void    ref_gl_Mod_Modellist_f (void);

void    *Hunk_Begin (int maxsize);
void    *Hunk_Alloc (int size);
int        Hunk_End (void);
void    Hunk_Free (void *base);

void    ref_gl_ModFreeAll (void);
void    ref_gl_ModFree (model_t *mod);

#endif // ____REF_GL_GL_MODEL_H__