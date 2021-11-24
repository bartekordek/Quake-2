#ifndef ____QCOMMON_QCOMMON_QFILES_H__
#define ____QCOMMON_QCOMMON_QFILES_H__

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

//
// qfiles.h: quake file formats
// This file must be identical in the quake and utils directories
//

/*
========================================================================

The .pak files are just a linear collapse of a directory tree

========================================================================
*/

#include "shared/dvis.h"

#define IDPAKHEADER ( ( 'K' << 24 ) + ( 'C' << 16 ) + ( 'A' << 8 ) + 'P' )

typedef struct
{
    char name[56];
    int filepos, filelen;
} dpackfile_t;

typedef struct
{
    int ident;  // == IDPAKHEADER
    int dirofs;
    int dirlen;
} dpackheader_t;

#define MAX_FILES_IN_PACK 4096

/*
========================================================================

.MD2 triangle model file format

========================================================================
*/

#define IDALIASHEADER ( ( '2' << 24 ) + ( 'P' << 16 ) + ( 'D' << 8 ) + 'I' )
#define ALIAS_VERSION 8

typedef struct
{
    short s;
    short t;
} dstvert_t;

typedef struct
{
    short index_xyz[3];
    short index_st[3];
} dtriangle_t;

#define DTRIVERTX_V0 0
#define DTRIVERTX_V1 1
#define DTRIVERTX_V2 2
#define DTRIVERTX_LNI 3
#define DTRIVERTX_SIZE 4



/*
========================================================================

.SP2 sprite file format

========================================================================
*/

#define IDSPRITEHEADER ( ( '2' << 24 ) + ( 'S' << 16 ) + ( 'D' << 8 ) + 'I' )
// little-endian "IDS2"
#define SPRITE_VERSION 2

typedef struct
{
    int width, height;
    int origin_x, origin_y;   // raster coordinates inside pic
    char name[MAX_SKINNAME];  // name of pcx file
} dsprframe_t;

typedef struct
{
    int ident;
    int version;
    int numframes;
    dsprframe_t frames[1];  // variable sized
} dsprite_t;

/*
==============================================================================

  .BSP file format

==============================================================================
*/

#define IDBSPHEADER ( ( 'P' << 24 ) + ( 'S' << 16 ) + ( 'B' << 8 ) + 'I' )
// little-endian "IBSP"

#define BSPVERSION 38

// upper design bounds
// leaffaces, leafbrushes, planes, and verts are still bounded by
// 16 bit short limits
#define MAX_MAP_MODELS 1024
#define MAX_MAP_BRUSHES 8192
#define MAX_MAP_ENTITIES 2048
#define MAX_MAP_ENTSTRING 0x40000
#define MAX_MAP_TEXINFO 8192

#define MAX_MAP_AREAS 256
#define MAX_MAP_AREAPORTALS 1024
#define MAX_MAP_PLANES 65536
#define MAX_MAP_NODES 65536
#define MAX_MAP_BRUSHSIDES 65536
#define MAX_MAP_LEAFS 65536
#define MAX_MAP_VERTS 65536
#define MAX_MAP_FACES 65536
#define MAX_MAP_LEAFFACES 65536
#define MAX_MAP_LEAFBRUSHES 65536
#define MAX_MAP_PORTALS 65536
#define MAX_MAP_EDGES 128000
#define MAX_MAP_SURFEDGES 256000
#define MAX_MAP_LIGHTING 0x200000
#define MAX_MAP_VISIBILITY 0x100000

// key / value pair sizes

#define MAX_KEY 32
#define MAX_VALUE 1024

//=============================================================================

typedef struct
{
    int fileofs, filelen;
} lump_t;

#define LUMP_ENTITIES 0
#define LUMP_PLANES 1
#define LUMP_VERTEXES 2
#define LUMP_VISIBILITY 3
#define LUMP_NODES 4
#define LUMP_TEXINFO 5
#define LUMP_FACES 6
#define LUMP_LIGHTING 7
#define LUMP_LEAFS 8
#define LUMP_LEAFFACES 9
#define LUMP_LEAFBRUSHES 10
#define LUMP_EDGES 11
#define LUMP_SURFEDGES 12
#define LUMP_MODELS 13
#define LUMP_BRUSHES 14
#define LUMP_BRUSHSIDES 15
#define LUMP_POP 16
#define LUMP_AREAS 17
#define LUMP_AREAPORTALS 18
#define HEADER_LUMPS 19

typedef struct
{
    int ident;
    int version;
    lump_t lumps[HEADER_LUMPS];
} dheader_t;

typedef struct
{
    float mins[3], maxs[3];
    float origin[3];  // for sounds or lights
    int headnode;
    int firstface, numfaces;  // submodels just draw faces
                              // without walking the bsp tree
} dmodel_t;

typedef struct
{
    float point[3];
} dvertex_t;

// 0-2 are axial planes
#define PLANE_X 0
#define PLANE_Y 1
#define PLANE_Z 2

// 3-5 are non-axial planes snapped to the nearest
#define PLANE_ANYX 3
#define PLANE_ANYY 4
#define PLANE_ANYZ 5

// planes (x&~1) and (x&~1)+1 are always opposites

typedef struct
{
    float normal[3];
    float dist;
    int type;  // PLANE_X - PLANE_ANYZ ?remove? trivial to regenerate
} dplane_t;

typedef struct
{
    int planenum;
    int children[2];  // negative numbers are -(leafs+1), not nodes
    short mins[3];    // for frustom culling
    short maxs[3];
    unsigned short firstface;
    unsigned short numfaces;  // counting both sides
} dnode_t;

typedef struct texinfo_s
{
    float vecs[2][4];  // [s/t][xyz offset]
    int flags;         // miptex flags + overrides
    int value;         // light emission, etc
    char texture[32];  // texture name (textures/*.wal)
    int nexttexinfo;   // for animations, -1 = end of chain
} texinfo_t;

// note that edge 0 is never used, because negative edge nums are used for
// counterclockwise use of the edge in a face
typedef struct
{
    unsigned short v[2];  // vertex numbers
} dedge_t;
typedef struct
{
    unsigned short planenum;
    short side;

    int firstedge;  // we must support > 64k edges
    short numedges;
    short texinfo;

    // lighting info
    byte styles[MAXLIGHTMAPS];
    int lightofs;  // start of [numstyles*surfsize] samples
} dface_t;

typedef struct
{
    int contents;  // OR of all brushes (not needed?)

    short cluster;
    short area;

    short mins[3];  // for frustum culling
    short maxs[3];

    unsigned short firstleafface;
    unsigned short numleaffaces;

    unsigned short firstleafbrush;
    unsigned short numleafbrushes;
} dleaf_t;

typedef struct
{
    unsigned short planenum;  // facing out of the leaf
    short texinfo;
} dbrushside_t;

typedef struct
{
    int firstside;
    int numsides;
    int contents;
} dbrush_t;

#define ANGLE_UP -1
#define ANGLE_DOWN -2

// the visibility lump consists of a header with a count, then
// byte offsets for the PVS and PHS of each cluster, then the raw
// compressed bit vectors
#define DVIS_PVS 0
#define DVIS_PHS 1

// each area has a list of portals that lead into other areas
// when portals are closed, other areas may not be visible or
// hearable even if the vis info says that it should be
typedef struct
{
    int portalnum;
    int otherarea;
} dareaportal_t;

typedef struct
{
    int numareaportals;
    int firstareaportal;
} darea_t;

#endif  // ____QCOMMON_QCOMMON_QFILES_H__