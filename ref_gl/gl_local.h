#ifndef ____REF_GL_GL_LOCAL_H__
#define ____REF_GL_GL_LOCAL_H__

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
// disable data conversion warnings

#if 0
#pragma warning(disable : 4244)     // MIPS
#pragma warning(disable : 4136)     // X86
#pragma warning(disable : 4051)     // ALPHA
#endif

#include "shared/viddef.h"
#include "shared/enum_image_type.h"
#include "client/client.h"

#ifdef _WIN32
#  include <windows.h>
#endif

#include <stdio.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>

#ifndef GL_COLOR_INDEX8_EXT
#define GL_COLOR_INDEX8_EXT GL_COLOR_INDEX
#endif

#include "client/ref.h"
#include "shared/enum_image_type.h"
#include "shared/image.h"

#include "qgl.h"

#define    REF_VERSION    "GL 0.01"

// up / down
#define    PITCH    0

// left / right
#define    YAW        1

// fall over
#define    ROLL    2


extern    viddef_t    vid;


/*

  skins will be outline flood filled and mip mapped
  pics and sprites with alpha will be outline flood filled
  pic won't be mip mapped

  model skin
  sprite frame
  wall texture
  pic

*/

#define TEXNUM_LIGHTMAPS    1024
#define TEXNUM_SCRAPS        1152
#define TEXNUM_IMAGES        1153

#define MAX_GLTEXTURES    1024

//===================================================================

#include "gl_model.h"

void GL_BeginRendering (int *x, int *y, int *width, int *height);
void GL_EndRendering (void);

void GL_SetDefaultState( void );
void GL_UpdateSwapInterval( void );

extern    float    gldepthmin, gldepthmax;

typedef struct
{
    float    x, y, z;
    float    s, t;
    float    r, g, b;
} glvert_t;


#define    MAX_LBM_HEIGHT        480

#define BACKFACE_EPSILON    0.01


//====================================================

extern    image_t        gltextures[MAX_GLTEXTURES];
extern    int            numgltextures;


extern    image_t        *r_notexture;
extern    image_t        *r_particletexture;
extern    entity_t    *currententity;
extern    model_t        *currentmodel;
extern    int            r_visframecount;
extern    int            r_framecount;
extern    plane_t    frustum[4];
extern    int            c_brush_polys, c_alias_polys;


extern    int            gl_filter_min, gl_filter_max;

//
// view origin
//
extern    vec3_t    vup;
extern    vec3_t    vpn;
extern    vec3_t    vright;
extern    vec3_t    r_origin;

//
// screen size info
//
extern    refdef_t    r_newrefdef;
extern    int        r_viewcluster, r_viewcluster2, r_oldviewcluster, r_oldviewcluster2;

extern    cvar_t    *r_norefresh;
extern    cvar_t    *r_lefthand;
extern    cvar_t    *r_drawentities;
extern    cvar_t    *r_drawworld;
extern    cvar_t    *r_speeds;
extern    cvar_t    *r_fullbright;
extern    cvar_t    *r_novis;
extern    cvar_t    *r_nocull;
extern    cvar_t    *r_lerpmodels;

extern    cvar_t    *r_lightlevel;    // FIXME: This is a HACK to get the client's light level

extern cvar_t    *gl_vertex_arrays;

extern cvar_t    *gl_ext_swapinterval;
extern cvar_t    *gl_ext_palettedtexture;
extern cvar_t    *gl_ext_multitexture;
extern cvar_t    *gl_ext_pointparameters;
extern cvar_t    *gl_ext_compiled_vertex_array;

extern cvar_t    *gl_particle_min_size;
extern cvar_t    *gl_particle_max_size;
extern cvar_t    *gl_particle_size;
extern cvar_t    *gl_particle_att_a;
extern cvar_t    *gl_particle_att_b;
extern cvar_t    *gl_particle_att_c;

extern    cvar_t    *gl_nosubimage;
extern    cvar_t    *gl_bitdepth;
extern    cvar_t    *gl_mode;
extern    cvar_t    *gl_log;
extern    cvar_t    *gl_lightmap;
extern    cvar_t    *gl_shadows;
extern    cvar_t    *gl_dynamic;
extern  cvar_t  *gl_monolightmap;
extern    cvar_t    *gl_nobind;
extern    cvar_t    *gl_round_down;
extern    cvar_t    *gl_picmip;
extern    cvar_t    *gl_skymip;
extern    cvar_t    *gl_showtris;
extern    cvar_t    *gl_finish;
extern    cvar_t    *gl_ztrick;
extern    cvar_t    *gl_clear;
extern    cvar_t    *gl_cull;
extern    cvar_t    *gl_poly;
extern    cvar_t    *gl_texsort;
extern    cvar_t    *gl_polyblend;
extern    cvar_t    *gl_flashblend;
extern    cvar_t    *gl_lightmaptype;
extern    cvar_t    *gl_modulate;
extern    cvar_t    *gl_playermip;
extern    cvar_t    *gl_drawbuffer;
extern    cvar_t    *gl_3dlabs_broken;
extern  cvar_t  *gl_driver;
extern    cvar_t    *gl_swapinterval;
extern    cvar_t    *gl_texturemode;
extern    cvar_t    *gl_texturealphamode;
extern    cvar_t    *gl_texturesolidmode;
extern  cvar_t  *gl_saturatelighting;
extern  cvar_t  *gl_lockpvs;

extern    cvar_t    *vid_fullscreen;
extern    cvar_t    *vid_gamma;

extern    cvar_t        *intensity;

extern    int        gl_lightmap_format;
extern    int        gl_solid_format;
extern    int        gl_alpha_format;
extern    int        gl_tex_solid_format;
extern    int        gl_tex_alpha_format;

extern    int        c_visible_lightmaps;
extern    int        c_visible_textures;

extern    float    r_world_matrix[16];

void R_TranslatePlayerSkin (int playernum);
void ref_gl_GL_Bind (int texnum);
void ref_gl_GL_MBind( GLenum target, int texnum );
void ref_gl_GL_TexEnv();
void ref_gl_GL_EnableMultitexture( qboolean enable );
void GL_SelectTexture( GLenum );

void ref_gl_R_LightPoint (vec3_t p, vec3_t color);
void ref_gl_R_PushDlights (void);

//====================================================================

extern    model_t    *r_worldmodel;

extern    unsigned    d_8to24table[256];

extern    int        registration_sequence;


void ref_gl_V_AddBlend (float r, float g, float b, float a, float *v_blend);

int  ref_gl_R_Init( void *hinstance, void *hWnd );
void ref_gl_R_Shutdown( void );

void ref_gl_R_RenderView (refdef_t *fd);
void ref_gl_GL_ScreenShot_f (void);
void ref_gl_R_DrawAliasModel (entity_t *e);
void ref_gl_R_DrawBrushModel (entity_t *e);
void ref_gl_R_DrawSpriteModel (entity_t *e);
void ref_gl_R_DrawBeam( entity_t *e );
void ref_gl_R_DrawWorld (void);
void ref_gl_R_RenderDlights (void);
void ref_gl_R_DrawAlphaSurfaces (void);
void ref_gl_R_RenderBrushPoly (msurface_t *fa);
void ref_gl_R_InitParticleTexture (void);
void ref_gl_Draw_InitLocal (void);
void ref_gl_GL_SubdivideSurface (msurface_t *fa);
qboolean ref_gl_R_CullBox (vec3_t mins, vec3_t maxs);
void ref_gl_R_RotateForEntity (entity_t *e);
void ref_gl_R_MarkLeaves (void);

glpoly_t *ref_gl_WaterWarpPolyVerts (glpoly_t *p);
void ref_gl_EmitWaterPolys (msurface_t *fa);
void ref_gl_R_AddSkySurface (msurface_t *fa);
void ref_gl_R_ClearSkyBox (void);
void ref_gl_R_DrawSkyBox (void);
void ref_gl_R_MarkLights (dlight_t *light, int bit, mnode_t *node);

void ref_gl_Mod_Init(void);

#if 0
short LittleShort (short l);
short BigShort (short l);
int    LittleLong (int l);
float LittleFloat (float f);

char    *va(char *format, ...);
// does a varargs printf into a temp buffer
#endif

void ref_gl_COM_StripExtension (char *in, char *out);

void ref_gl_Draw_GetPicSize(int *w, int *h, char *name);
void ref_gl_Draw_Pic(int x, int y, char *name);
void ref_gl_Draw_StretchPic (int x, int y, int w, int h, char *name);
void ref_gl_Draw_Char (int x, int y, int c);
void ref_gl_Draw_TileClear (int x, int y, int w, int h, char *name);
void ref_gl_Draw_Fill (int x, int y, int w, int h, int c);
void ref_gl_Draw_FadeScreen (void);
void ref_gl_Draw_StretchRaw (int x, int y, int w, int h, int cols, int rows, byte *data);

void ref_gl_R_BeginFrame( float camera_separation );
void ref_gl_R_SwapBuffers( int );
void ref_gl_R_SetPalette ( const unsigned char *palette);

void ref_gl_GL_ResampleTexture (unsigned *in, int inwidth, int inheight, unsigned *out,  int outwidth, int outheight);

struct image_s* ref_gl_R_RegisterSkin (char *name);

void ref_gl_LoadPCX (char *filename, byte **pic, byte **palette, int *width, int *height);
image_t *ref_gl_GL_LoadPic (char *name, byte *pic, int width, int height, imagetype_t type, int bits);
image_t* ref_gl_GL_FindImage (char *name, imagetype_t type);
void ref_gl_GL_TextureMode( char *string );
void ref_gl_GL_ImageList_f (void);

void ref_gl_GL_SetTexturePalette( unsigned palette[256] );

void ref_gl_GL_InitImages (void);
void ref_gl_GL_ShutdownImages (void);

void ref_gl_GL_FreeUnusedImages (void);

void ref_gl_GL_TextureAlphaMode( char *string );
void ref_gl_GL_TextureSolidMode( char *string );

/*
** GL extension emulation functions
*/
void ref_gl_GL_DrawParticles( int n, const particle_t particles[], const unsigned colortable[768] );

/*
** GL config stuff
*/
#define GL_RENDERER_VOODOO        0x00000001
#define GL_RENDERER_VOODOO2       0x00000002
#define GL_RENDERER_VOODOO_RUSH    0x00000004
#define GL_RENDERER_BANSHEE        0x00000008
#define        GL_RENDERER_3DFX        0x0000000F

#define GL_RENDERER_PCX1        0x00000010
#define GL_RENDERER_PCX2        0x00000020
#define GL_RENDERER_PMX            0x00000040
#define        GL_RENDERER_POWERVR        0x00000070

#define GL_RENDERER_PERMEDIA2    0x00000100
#define GL_RENDERER_GLINT_MX    0x00000200
#define GL_RENDERER_GLINT_TX    0x00000400
#define GL_RENDERER_3DLABS_MISC    0x00000800
#define        GL_RENDERER_3DLABS    0x00000F00

#define GL_RENDERER_REALIZM        0x00001000
#define GL_RENDERER_REALIZM2    0x00002000
#define        GL_RENDERER_INTERGRAPH    0x00003000

#define GL_RENDERER_3DPRO        0x00004000
#define GL_RENDERER_REAL3D        0x00008000
#define GL_RENDERER_RIVA128        0x00010000
#define GL_RENDERER_DYPIC        0x00020000

#define GL_RENDERER_V1000        0x00040000
#define GL_RENDERER_V2100        0x00080000
#define GL_RENDERER_V2200        0x00100000
#define        GL_RENDERER_RENDITION    0x001C0000

#define GL_RENDERER_O2          0x00100000
#define GL_RENDERER_IMPACT      0x00200000
#define GL_RENDERER_RE            0x00400000
#define GL_RENDERER_IR            0x00800000
#define        GL_RENDERER_SGI            0x00F00000

#define GL_RENDERER_MCD            0x01000000
#define GL_RENDERER_OTHER        0x80000000

typedef struct
{
    int         renderer;
    const char *renderer_string;
    const char *vendor_string;
    const char *version_string;
    const char *extensions_string;

    qboolean    allow_cds;
} glconfig_t;

typedef struct
{
    float inverse_intensity;
    qboolean fullscreen;

    int     prev_mode;

    unsigned char *d_16to8table;

    int lightmap_textures;

    int    currenttextures[2];
    int currenttmu;

    float camera_separation;
    qboolean stereo_enabled;

    unsigned char originalRedGammaTable[256];
    unsigned char originalGreenGammaTable[256];
    unsigned char originalBlueGammaTable[256];
} glstate_t;

extern glconfig_t  gl_config;
extern glstate_t   gl_state;

/*
====================================================================

IMPORTED FUNCTIONS

====================================================================
*/

extern    refimport_t    ri;


/*
====================================================================

IMPLEMENTATION SPECIFIC FUNCTIONS

====================================================================
*/

void        GLimp_BeginFrame( float camera_separation );
void        GLimp_EndFrame( void );
int         GLimp_Init( void *hinstance, void *hWnd );
void        GLimp_Shutdown( void );
int         GLimp_SetMode( int *pwidth, int *pheight, int mode, qboolean fullscreen );
void        GLimp_AppActivate( qboolean active );
void        GLimp_EnableLogging( qboolean enable );
void        GLimp_LogNewFrame( void );

refexport_t ref_gl_GetRefAPI( refimport_t rimp );

#endif // ____REF_GL_GL_LOCAL_H__
