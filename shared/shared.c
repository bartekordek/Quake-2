#include "shared/shared.h"

#include <stdarg.h>
#include <stdio.h>

cvar_t* gl_bitdepth;
cvar_t* gl_log;
cvar_t* gl_drawbuffer;
cvar_t* gl_driver;
cvar_t* gl_lightmap;
cvar_t* gl_shadows;
cvar_t* gl_mode;
cvar_t* gl_dynamic;
cvar_t* gl_monolightmap;
cvar_t* gl_modulate;
cvar_t* gl_nobind;
cvar_t* gl_round_down;
cvar_t* gl_picmip;
cvar_t* gl_skymip;
cvar_t* gl_showtris;
cvar_t* gl_ztrick;
cvar_t* gl_finish;
cvar_t* gl_clear;
cvar_t* gl_cull;
cvar_t* gl_polyblend;
cvar_t* gl_flashblend;
cvar_t* gl_playermip;
cvar_t* gl_saturatelighting;
cvar_t* gl_swapinterval;
cvar_t* gl_texturemode;
cvar_t* gl_texturealphamode;
cvar_t* gl_texturesolidmode;
cvar_t* gl_lockpvs;

glstate_t gl_state;

refimport_t ri;

void Com_Printf( char* msg, ... )
{
    // TODO:
    // va_list argptr;
    // char text[1024];

    // va_start( argptr, msg );
    // vsprintf( text, msg, argptr );
    // va_end( argptr );

    // gi.dprintf( "%s", text );
}

void Com_sprintf( char* dest, int size, char* fmt, ... )
{
    int len;
    va_list argptr;
    char bigbuffer[0x10000];

    va_start( argptr, fmt );
    len = vsprintf( bigbuffer, fmt, argptr );
    va_end( argptr );
    if( len >= size )
        Com_Printf( "Com_sprintf: overflow of %i in %i\n", len, size );
    strncpy( dest, bigbuffer, size - 1 );
}