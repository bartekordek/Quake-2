#ifndef ____SHARED_SHARED_SHARED_H__
#define ____SHARED_SHARED_SHARED_H__

#include "cvar.h"
#include "shared/glstate.h"
#include "shared/refimport.h"

extern cvar_t* gl_bitdepth;
extern cvar_t* gl_nosubimage;
extern cvar_t* gl_mode;
extern cvar_t* gl_log;
extern cvar_t* gl_lightmap;
extern cvar_t* gl_shadows;
extern cvar_t* gl_dynamic;
extern cvar_t* gl_monolightmap;
extern cvar_t* gl_nobind;
extern cvar_t* gl_round_down;
extern cvar_t* gl_picmip;
extern cvar_t* gl_skymip;
extern cvar_t* gl_showtris;
extern cvar_t* gl_finish;
extern cvar_t* gl_ztrick;
extern cvar_t* gl_clear;
extern cvar_t* gl_cull;
extern cvar_t* gl_poly;
extern cvar_t* gl_texsort;
extern cvar_t* gl_polyblend;
extern cvar_t* gl_flashblend;
extern cvar_t* gl_lightmaptype;
extern cvar_t* gl_modulate;
extern cvar_t* gl_playermip;
extern cvar_t* gl_drawbuffer;
extern cvar_t* gl_3dlabs_broken;
extern cvar_t* gl_driver;
extern cvar_t* gl_swapinterval;
extern cvar_t* gl_texturemode;
extern cvar_t* gl_texturealphamode;
extern cvar_t* gl_texturesolidmode;
extern cvar_t* gl_saturatelighting;
extern cvar_t* gl_lockpvs;
extern glstate_t gl_state;

/*
====================================================================

IMPORTED FUNCTIONS

====================================================================
*/

extern refimport_t ri;

__declspec( dllexport ) void Com_Printf( char* msg, ... );
__declspec( dllexport ) void Com_sprintf( char* dest, int size, char* fmt, ... );

#endif  // ____SHARED_SHARED_SHARED_H__