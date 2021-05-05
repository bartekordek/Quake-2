#ifndef ____REF_SOFT_R_MODEL_H__
#define ____REF_SOFT_R_MODEL_H__

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

#include "shared/defines.hpp"
#include "shared/mtexinfo.hpp"
#include "shared/plane.hpp"
#include "shared/msurface.hpp"
#include "shared/mnode.hpp"
#include "shared/mleaf.hpp"
#include "shared/model.hpp"

#include "qcommon/qfiles.hpp"

/*

d*_t structures are on-disk representations
m*_t structures are in-memory

*/


/*
==============================================================================

BRUSH MODELS

==============================================================================
*/


//
// in memory representation
//
// !!! if this is changed, it must be changed in asm_draw.h too !!!


#define    SIDE_FRONT    0
#define    SIDE_BACK    1
#define    SIDE_ON        2




// FIXME: differentiate from texinfo SURF_ flags
#define    SURF_PLANEBACK        2
#define    SURF_DRAWSKY        4            // sky brush face
#define SURF_DRAWTURB        0x10
#define SURF_DRAWBACKGROUND    0x40
#define SURF_DRAWSKYBOX        0x80        // sky box

#define SURF_FLOW            0x100        //PGM

// !!! if this is changed, it must be changed in asm_draw.h too !!!

#define    CONTENTS_NODE    -1
//
// Whole model
//

//============================================================================

void    ref_soft_Mod_Init (void);
void    ref_soft_Mod_ClearAll (void);
model_t *ref_soft_Mod_ForName (char *name, bool crash);
void    *Mod_Extradata (model_t *mod);    // handles caching
void    ref_soft_Mod_TouchModel (char *name);

mleaf_t* ref_soft_Mod_PointInLeaf (float *p, model_t *model);
byte* ref_soft_Mod_ClusterPVS (int cluster, model_t *model);

void ref_soft_Mod_Modellist_f (void);
void ref_soft_Mod_FreeAll (void);
void ref_soft_Mod_Free (model_t *mod);

extern    int        registration_sequence;

#endif // ____REF_SOFT_R_MODEL_H__