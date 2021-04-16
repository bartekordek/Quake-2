#ifndef ____SHARED_SHARED_FUNCTIONS_H__
#define ____SHARED_SHARED_FUNCTIONS_H__



#include "shared/defines.hpp"

#include "game/g_local.hpp"
#include "ref_gl/gl_local.hpp"
#include "server/server.hpp"
#include "qcommon/qcommon.hpp"

extern refimport_t ri;
extern game_import gi;
extern FILE	*logfile;
extern cvar	*logfile_active;
extern char	*rd_buffer;
extern int    rd_buffersize;
extern server_static_t    svs;

// this is only here so the functions in q_shared.c and q_shwin.c can link
void Sys_Error_G(char *error, ...);
void Com_Printf_G(char *msg, ...);

void Sys_Error_R(char *error, ...);
void Com_Printf_R(char *msg, ...);

// void Com_Printf_C(char *fmt, ...);

bool VID_CreateWindow_0( int width, int height, bool fullscreen );
void VID_CreateWindow_1( int width, int height, int stylebits );

void SV_Shutdown (char *finalmsg, bool reconnect);

void SV_FlushRedirect (int sv_redirected, char *outputbuf);

extern float    r_avertexnormals[NUMVERTEXNORMALS][3];

int ref_gl_Draw_GetPalette (void);
void ref_soft_Draw_GetPalette (void);

namespace Math
{
float abs( float val );
}

#endif // ____SHARED_SHARED_FUNCTIONS_H__
