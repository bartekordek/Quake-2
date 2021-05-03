#ifndef ____SHARED_SHARED_FUNCTIONS_H__
#define ____SHARED_SHARED_FUNCTIONS_H__

#include "game/g_local.h"
#include "qcommon/qcommon.h"
#include "ref_gl/gl_local.h"
#include "server/server.h"
#include "shared/defines.h"

extern refimport_t ri;
extern game_import_t gi;
extern FILE* logfile;
extern cvar_t* logfile_active;
extern char* rd_buffer;
extern int rd_buffersize;
extern server_static_t svs;

// this is only here so the functions in q_shared.c and q_shwin.c can link
void Sys_Error_G( char* error, ... );
void Com_Printf_G( char* msg, ... );

void Sys_Error_R( char* error, ... );
void Com_Printf_R( char* msg, ... );

void Com_Printf_C( char* fmt, ... );

qboolean VID_CreateWindow_0( int width, int height, qboolean fullscreen );
void VID_CreateWindow_1( int width, int height, int stylebits );

void SV_Shutdown( char* finalmsg, qboolean reconnect );

void SV_FlushRedirect( int sv_redirected, char* outputbuf );

extern float r_avertexnormals[NUMVERTEXNORMALS][3];

int ref_gl_Draw_GetPalette( void );
void ref_soft_Draw_GetPalette( void );

int NumForEdict( struct edict_s* e );
edict_t* EdictNum( int n );

#endif  // ____SHARED_SHARED_FUNCTIONS_H__
