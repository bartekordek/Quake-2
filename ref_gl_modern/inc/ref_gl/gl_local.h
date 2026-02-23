#ifndef __ref_gl_gl_local_h__
#define __ref_gl_gl_local_h__

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

#include "shared/config.h"
#include "shared/image.h"

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

#include "shared/ref.h"

#include "qgl.h"

#define	REF_VERSION	"GL 0.01"

// up / down
#define	PITCH	0

// left / right
#define	YAW		1

// fall over
#define	ROLL	2


typedef struct
{
	unsigned		width, height;			// coordinates from main game
} viddef_t;

EXTERNC	viddef_t	vid;


/*

  skins will be outline flood filled and mip mapped
  pics and sprites with alpha will be outline flood filled
  pic won't be mip mapped

  model skin
  sprite frame
  wall texture
  pic

*/




#define	TEXNUM_LIGHTMAPS	1024
#define	TEXNUM_SCRAPS		1152
#define	TEXNUM_IMAGES		1153

#define		MAX_GLTEXTURES	1024

//===================================================================

typedef enum
{
	rserr_ok,

	rserr_invalid_fullscreen,
	rserr_invalid_mode,

	rserr_unknown
} rserr_t;

#include "gl_model.h"

EXTERNC void  GL_BeginRendering (int *x, int *y, int *width, int *height);
EXTERNC void  GL_EndRendering (void);
EXTERNC void  GL_SetDefaultState (void);
EXTERNC void  GL_UpdateSwapInterval (void);
EXTERNC float gldepthmin, gldepthmax;

typedef struct
{
	float	x, y, z;
	float	s, t;
	float	r, g, b;
} glvert_t;


#define	MAX_LBM_HEIGHT		480

#define BACKFACE_EPSILON	0.01


//====================================================

EXTERNC	image_t		gltextures[MAX_GLTEXTURES];
EXTERNC	int			numgltextures;


EXTERNC	image_t		*r_notexture;
EXTERNC	image_t		*r_particletexture;
EXTERNC	entity_t	*currententity;
EXTERNC	model_t		*currentmodel;
EXTERNC	int			r_visframecount;
EXTERNC	int			r_framecount;
EXTERNC	cplane_t	frustum[4];
EXTERNC	int			c_brush_polys, c_alias_polys;


EXTERNC	int			gl_filter_min, gl_filter_max;

//
// view origin
//
EXTERNC	vec3_t	vup;
EXTERNC	vec3_t	vpn;
EXTERNC	vec3_t	vright;
EXTERNC	vec3_t	r_origin;

//
// screen size info
//
EXTERNC	refdef_t	r_newrefdef;
EXTERNC	int		r_viewcluster, r_viewcluster2, r_oldviewcluster, r_oldviewcluster2;

EXTERNC	cvar_t	*r_norefresh;
EXTERNC	cvar_t	*r_lefthand;
EXTERNC	cvar_t	*r_drawentities;
EXTERNC	cvar_t	*r_drawworld;
EXTERNC	cvar_t	*r_speeds;
EXTERNC	cvar_t	*r_fullbright;
EXTERNC	cvar_t	*r_novis;
EXTERNC	cvar_t	*r_nocull;
EXTERNC	cvar_t	*r_lerpmodels;

EXTERNC	cvar_t	*r_lightlevel;	// FIXME: This is a HACK to get the client's light level

EXTERNC cvar_t	*gl_vertex_arrays;

EXTERNC cvar_t	*gl_ext_swapinterval;
EXTERNC cvar_t	*gl_ext_palettedtexture;
EXTERNC cvar_t	*gl_ext_multitexture;
EXTERNC cvar_t	*gl_ext_pointparameters;
EXTERNC cvar_t	*gl_ext_compiled_vertex_array;

EXTERNC cvar_t	*gl_particle_min_size;
EXTERNC cvar_t	*gl_particle_max_size;
EXTERNC cvar_t	*gl_particle_size;
EXTERNC cvar_t	*gl_particle_att_a;
EXTERNC cvar_t	*gl_particle_att_b;
EXTERNC cvar_t	*gl_particle_att_c;

EXTERNC	cvar_t	*gl_nosubimage;
EXTERNC	cvar_t	*gl_bitdepth;
EXTERNC cvar_t	*gl_mode;
EXTERNC	cvar_t	*gl_log;
EXTERNC	cvar_t	*gl_lightmap;
EXTERNC	cvar_t	*gl_shadows;
EXTERNC	cvar_t	*gl_dynamic;
EXTERNC cvar_t  *gl_monolightmap;
EXTERNC	cvar_t	*gl_nobind;
EXTERNC	cvar_t	*gl_round_down;
EXTERNC	cvar_t	*gl_picmip;
EXTERNC	cvar_t	*gl_skymip;
EXTERNC	cvar_t	*gl_showtris;
EXTERNC	cvar_t	*gl_finish;
EXTERNC	cvar_t	*gl_ztrick;
EXTERNC	cvar_t	*gl_clear;
EXTERNC	cvar_t	*gl_cull;
EXTERNC	cvar_t	*gl_poly;
EXTERNC	cvar_t	*gl_texsort;
EXTERNC	cvar_t	*gl_polyblend;
EXTERNC	cvar_t	*gl_flashblend;
EXTERNC	cvar_t	*gl_lightmaptype;
EXTERNC	cvar_t	*gl_modulate;
EXTERNC	cvar_t	*gl_playermip;
EXTERNC	cvar_t	*gl_drawbuffer;
EXTERNC	cvar_t	*gl_3dlabs_broken;
EXTERNC  cvar_t  *gl_driver;
EXTERNC	cvar_t	*gl_swapinterval;
EXTERNC	cvar_t	*gl_texturemode;
EXTERNC	cvar_t	*gl_texturealphamode;
EXTERNC	cvar_t	*gl_texturesolidmode;
EXTERNC  cvar_t  *gl_saturatelighting;
EXTERNC  cvar_t  *gl_lockpvs;

EXTERNC	cvar_t	*vid_fullscreen;
EXTERNC	cvar_t	*vid_gamma;

EXTERNC	cvar_t		*intensity;

EXTERNC	int		gl_lightmap_format;
EXTERNC	int		gl_solid_format;
EXTERNC	int		gl_alpha_format;
EXTERNC	int		gl_tex_solid_format;
EXTERNC	int		gl_tex_alpha_format;

EXTERNC	int		c_visible_lightmaps;
EXTERNC	int		c_visible_textures;

EXTERNC	float	r_world_matrix[16];
EXTERNC void	R_TranslatePlayerSkin (int playernum);
EXTERNC void	GL_BindTexture (int texnum);
EXTERNC void	GL_MBind (GLenum target, int texnum);
EXTERNC void	GL_TexEnv (GLenum value);
EXTERNC void	GL_EnableMultitexture (qboolean enable);
EXTERNC void	GL_SelectTexture (GLenum);
EXTERNC void	R_LightPoint (vec3_t p, vec3_t color);
EXTERNC void	R_PushDlights (void);

//====================================================================

EXTERNC	model_t	*r_worldmodel;

EXTERNC	unsigned	d_8to24table[256];

EXTERNC	int		registration_sequence;


void V_AddBlend (float r, float g, float b, float a, float *v_blend);

qboolean			 R_Init ();
void				 R_Shutdown (void);
EXTERNC void		 R_RenderView (refdef_t *fd);
EXTERNC void		 GL_ScreenShot_f (void);
EXTERNC void		 R_DrawAliasModel (entity_t *e);
EXTERNC void		 R_DrawBrushModel (entity_t *e);
EXTERNC void		 R_DrawSpriteModel (entity_t *e);
EXTERNC void		 R_DrawBeam (entity_t *e);
EXTERNC void		 R_DrawWorld (void);
EXTERNC void		 R_RenderDlights (void);
EXTERNC void		 R_DrawAlphaSurfaces (void);
EXTERNC void		 R_RenderBrushPoly (msurface_t *fa);
EXTERNC void				 R_InitParticleTexture (void);
EXTERNC void				 Draw_InitLocal (void);
EXTERNC void				 GL_SubdivideSurface (msurface_t *fa);
EXTERNC qboolean			 R_CullBox (vec3_t mins, vec3_t maxs);
EXTERNC void				 R_RotateForEntity (entity_t *e);
EXTERNC void				 R_MarkLeaves (void);
EXTERNC glpoly_t			*WaterWarpPolyVerts (glpoly_t *p);
EXTERNC void				 EmitWaterPolys (msurface_t *fa);
EXTERNC void				 R_AddSkySurface (msurface_t *fa);
EXTERNC void				 R_ClearSkyBox (void);
EXTERNC void				 R_DrawSkyBox (void);
EXTERNC void				 R_MarkLights (dlight_t *light, int bit, mnode_t *node);

#if 0
short LittleShort (short l);
short BigShort (short l);
int	LittleLong (int l);
float LittleFloat (float f);

char	*va(const char *format, ...);
// does a varargs printf into a temp buffer
#endif

void COM_StripExtension (const char *in, char *out);

EXTERNC void Draw_GetPicSize (int *w, int *h, char *name);
EXTERNC void Draw_Pic (int x, int y, char *name);
EXTERNC void Draw_StretchPic (int x, int y, int w, int h, char *name);
EXTERNC void	Draw_Char (int x, int y, int c);
EXTERNC void	Draw_TileClear (int x, int y, int w, int h, char *name);
EXTERNC void	Draw_Fill (int x, int y, int w, int h, int c);
EXTERNC void	Draw_FadeScreen (void);
EXTERNC void	Draw_StretchRaw (int x, int y, int w, int h, int cols, int rows, byte *data);
EXTERNC void			R_BeginFrame (float camera_separation);
EXTERNC void			R_SwapBuffers (int);
EXTERNC void			R_SetPalette (const unsigned char *palette);
EXTERNC int				Draw_GetPalette (void);
EXTERNC void			GL_ResampleTexture (unsigned *in, int inwidth, int inheight, unsigned *out, int outwidth, int outheight);
EXTERNC struct image_s *R_RegisterSkin (const char *name);

void LoadPCX (const char *filename, byte **pic, byte **palette, int *width, int *height);
EXTERNC image_t	*GL_FindImage (const char *name, imagetype_t type);
EXTERNC void	GL_TextureMode( char *string );
EXTERNC void	GL_ImageList_f (void);
EXTERNC void	GL_SetTexturePalette( unsigned palette[256] );
EXTERNC void	GL_InitImages (void);
EXTERNC void	GL_ShutdownImages (void);
EXTERNC void	GL_FreeUnusedImages (void);
EXTERNC void GL_TextureAlphaMode( char *string );
EXTERNC void GL_TextureSolidMode( char *string );

/*
** GL extension emulation functions
*/
void GL_DrawParticles( int n, const particle_t particles[], const unsigned colortable[768] );

/*
** GL config stuff
*/
#define GL_RENDERER_VOODOO		0x00000001
#define GL_RENDERER_VOODOO2   	0x00000002
#define GL_RENDERER_VOODOO_RUSH	0x00000004
#define GL_RENDERER_BANSHEE		0x00000008
#define		GL_RENDERER_3DFX		0x0000000F

#define GL_RENDERER_PCX1		0x00000010
#define GL_RENDERER_PCX2		0x00000020
#define GL_RENDERER_PMX			0x00000040
#define		GL_RENDERER_POWERVR		0x00000070

#define GL_RENDERER_PERMEDIA2	0x00000100
#define GL_RENDERER_GLINT_MX	0x00000200
#define GL_RENDERER_GLINT_TX	0x00000400
#define GL_RENDERER_3DLABS_MISC	0x00000800
#define		GL_RENDERER_3DLABS	0x00000F00

#define GL_RENDERER_REALIZM		0x00001000
#define GL_RENDERER_REALIZM2	0x00002000
#define		GL_RENDERER_INTERGRAPH	0x00003000

#define GL_RENDERER_3DPRO		0x00004000
#define GL_RENDERER_REAL3D		0x00008000
#define GL_RENDERER_RIVA128		0x00010000
#define GL_RENDERER_DYPIC		0x00020000

#define GL_RENDERER_V1000		0x00040000
#define GL_RENDERER_V2100		0x00080000
#define GL_RENDERER_V2200		0x00100000
#define		GL_RENDERER_RENDITION	0x001C0000

#define GL_RENDERER_O2          0x00100000
#define GL_RENDERER_IMPACT      0x00200000
#define GL_RENDERER_RE			0x00400000
#define GL_RENDERER_IR			0x00800000
#define		GL_RENDERER_SGI			0x00F00000

#define GL_RENDERER_MCD			0x01000000
#define GL_RENDERER_OTHER		0x80000000

typedef struct
{
	int         renderer;
	const char *renderer_string;
	const char *vendor_string;
	const char *version_string;
	const char *extensions_string;

	qboolean	allow_cds;
} glconfig_t;

typedef struct
{
	float inverse_intensity;
	qboolean fullscreen;

	int     prev_mode;

	unsigned char *d_16to8table;

	int lightmap_textures;

	int	currenttextures[2];
	int currenttmu;

	float camera_separation;
	qboolean stereo_enabled;

	unsigned char originalRedGammaTable[256];
	unsigned char originalGreenGammaTable[256];
	unsigned char originalBlueGammaTable[256];
} glstate_t;

EXTERNC glconfig_t  gl_config;
EXTERNC glstate_t   gl_state;

/*
====================================================================

IMPORTED FUNCTIONS

====================================================================
*/

EXTERNC	refimport_t	ri;


/*
====================================================================

IMPLEMENTATION SPECIFIC FUNCTIONS

====================================================================
*/

EXTERNC void		GLimp_BeginFrame (float camera_separation);
EXTERNC void		GLimp_EndFrame (void);
EXTERNC qboolean	GLimp_Init (void);
EXTERNC void		GLimp_Shutdown (void);
EXTERNC rserr_t		GLimp_SetMode (int *pwidth, int *pheight, int mode, qboolean fullscreen);
EXTERNC void		GLimp_AppActivate (qboolean active);
EXTERNC void		GLimp_EnableLogging (qboolean enable);
EXTERNC void		GLimp_LogNewFrame (void);


#endif	// __ref_gl_gl_local_h__