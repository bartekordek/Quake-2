#ifndef __SHARED_SHARED_FUNCTIONS__
#define __SHARED_SHARED_FUNCTIONS__

// this is only here so the functions in q_shared.c and q_shwin.c can link
void Sys_Error_G(char *error, ...);
void Com_Printf_G(char *msg, ...);

void Sys_Error_R(char *error, ...);
void Com_Printf_R(char *msg, ...);

void Com_Printf_C(char *fmt, ...);

#endif