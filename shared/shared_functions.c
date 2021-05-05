#include "shared/shared_functions.h"
#include "shared/edict.h"
#include "shared/shared_objects.h"

#ifdef _WIN32
#include <vadefs.h>
#else
#include <stdarg.h>
#endif

#if defined WIN32
#include "win32/rw_win.h"
#include "win32/glw_win.h"
#include "win32/winquake.h"
#endif

float r_avertexnormals[NUMVERTEXNORMALS][3] = {
#include "shared/anorms.h"
};


void Sys_Error_R(char *error, ...)
{
    va_list        argptr;
    char        text[1024];

    va_start (argptr, error);
    vsprintf (text, error, argptr);
    va_end (argptr);

    ri.Sys_Error (ERR_FATAL, "%s", text);
}

void Com_Printf_R (char *fmt, ...)
{
    va_list        argptr;
    char        text[1024];

    va_start (argptr, fmt);
    vsprintf (text, fmt, argptr);
    va_end (argptr);

    ri.Con_Printf (PRINT_ALL, "%s", text);
}


void Sys_Error_G(char *error, ...)
{
    va_list        argptr;
    char        text[1024];

    va_start (argptr, error);
    vsprintf (text, error, argptr);
    va_end (argptr);

    gi.error (ERR_FATAL, "%s", text);
}


void Com_Printf_G(char *msg, ...)
{
    va_list        argptr;
    char        text[1024];

    va_start (argptr, msg);
    vsprintf (text, msg, argptr);
    va_end (argptr);

    gi.dprintf ("%s", text);
}

/*
=============
Com_Printf_C

Both client and server can use this, and it will output
to the apropriate place.
=============
*/
void Com_Printf_C(char *fmt, ...)
{
    va_list        argptr;
    char        msg[MAXPRINTMSG];

    va_start (argptr,fmt);
    vsprintf (msg,fmt,argptr);
    va_end (argptr);

    if (rd_target)
    {
        if ((strlen (msg) + strlen(rd_buffer)) > (rd_buffersize - 1))
        {
            SV_FlushRedirect(rd_target, rd_buffer);
            *rd_buffer = 0;
        }
        strcat (rd_buffer, msg);
        return;
    }

    Con_Print (msg);

    // also echo to debugging console
    #ifdef _WIN32
    win32_Sys_ConsoleOutput (msg);
    #endif

    // logfile
    if (logfile_active && logfile_active->value)
    {
        char    name[MAX_QPATH];

        if (!logfile)
        {
            Com_sprintf (name, sizeof(name), "%s/qconsole.log", FS_Gamedir ());
            logfile = fopen (name, "w");
        }
        if (logfile)
            fprintf (logfile, "%s", msg);
        if (logfile_active->value > 1)
            fflush (logfile);        // force it to save every time
    }
}

#ifdef _WIN32
void VID_CreateWindow_1( int width, int height, int stylebits )
{
    WNDCLASS        wc;
    RECT            r;
    cvar_t            *vid_xpos, *vid_ypos, *vid_fullscreen;
    int                x, y, w, h;
    int                exstyle;

    vid_xpos = ri.Cvar_Get ("vid_xpos", "0", 0);
    vid_ypos = ri.Cvar_Get ("vid_ypos", "0", 0);
    vid_fullscreen = ri.Cvar_Get ("vid_fullscreen", "0", CVAR_ARCHIVE );

    if ( vid_fullscreen->value )
        exstyle = WS_EX_TOPMOST;
    else
        exstyle = 0;

    /* Register the frame class */
    wc.style         = 0;
    wc.lpfnWndProc   = (WNDPROC)sww_state.wndproc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = sww_state.hInstance;
    wc.hIcon         = 0;
    wc.hCursor       = LoadCursor (NULL,IDC_ARROW);
    wc.hbrBackground = (void *)COLOR_GRAYTEXT;
    wc.lpszMenuName  = 0;
    wc.lpszClassName = WINDOW_CLASS_NAME;

    if (!RegisterClass (&wc) )
        ri.Sys_Error (ERR_FATAL, "Couldn't register window class");

    r.left = 0;
    r.top = 0;
    r.right  = width;
    r.bottom = height;

    AdjustWindowRect (&r, stylebits, FALSE);

    w = r.right - r.left;
    h = r.bottom - r.top;
    x = vid_xpos->value;
    y = vid_ypos->value;

    sww_state.hWnd = CreateWindowEx (
        exstyle,
         WINDOW_CLASS_NAME,
         "Quake 2",
         stylebits,
         x, y, w, h,
         NULL,
         NULL,
         sww_state.hInstance,
         NULL);

    if (!sww_state.hWnd)
        ri.Sys_Error (ERR_FATAL, "Couldn't create window");

    ShowWindow( sww_state.hWnd, SW_SHOWNORMAL );
    UpdateWindow( sww_state.hWnd );
    SetForegroundWindow( sww_state.hWnd );
    SetFocus( sww_state.hWnd );

    // let the sound and input subsystems know about the new window
    ri.Vid_NewWindow (width, height);
}

qboolean VID_CreateWindow_0( int width, int height, qboolean fullscreen )
{
    WNDCLASS        wc;
    RECT            r;
    cvar_t            *vid_xpos, *vid_ypos;
    int                stylebits;
    int                x, y, w, h;
    int                exstyle;

    /* Register the frame class */
    wc.style         = 0;
    wc.lpfnWndProc   = (WNDPROC)glw_state.wndproc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = glw_state.hInstance;
    wc.hIcon         = 0;
    wc.hCursor       = LoadCursor (NULL,IDC_ARROW);
    wc.hbrBackground = (void *)COLOR_GRAYTEXT;
    wc.lpszMenuName  = 0;
    wc.lpszClassName = WINDOW_CLASS_NAME;

    if (!RegisterClass (&wc) )
        ri.Sys_Error (ERR_FATAL, "Couldn't register window class");

    if (fullscreen)
    {
        exstyle = WS_EX_TOPMOST;
        stylebits = WS_POPUP|WS_VISIBLE;
    }
    else
    {
        exstyle = 0;
        stylebits = WINDOW_STYLE;
    }

    r.left = 0;
    r.top = 0;
    r.right  = width;
    r.bottom = height;

    AdjustWindowRect (&r, stylebits, FALSE);

    w = r.right - r.left;
    h = r.bottom - r.top;

    if (fullscreen)
    {
        x = 0;
        y = 0;
    }
    else
    {
        vid_xpos = ri.Cvar_Get ("vid_xpos", "0", 0);
        vid_ypos = ri.Cvar_Get ("vid_ypos", "0", 0);
        x = vid_xpos->value;
        y = vid_ypos->value;
    }

    glw_state.hWnd = CreateWindowEx (
         exstyle,
         WINDOW_CLASS_NAME,
         "Quake 2",
         stylebits,
         x, y, w, h,
         NULL,
         NULL,
         glw_state.hInstance,
         NULL);

    if (!glw_state.hWnd)
        ri.Sys_Error (ERR_FATAL, "Couldn't create window");

    ShowWindow( glw_state.hWnd, SW_SHOW );
    UpdateWindow( glw_state.hWnd );

    // init all the gl stuff for the window
    if (!GLimp_InitGL ())
    {
        ri.Con_Printf( PRINT_ALL, "VID_CreateWindow() - GLimp_InitGL failed\n");
        return false;
    }

    SetForegroundWindow( glw_state.hWnd );
    SetFocus( glw_state.hWnd );

    // let the sound and input subsystems know about the new window
    ri.Vid_NewWindow (width, height);

    return true;
}
#endif

/*
================
SV_Shutdown

Called when each game quits,
before Sys_Quit or Sys_Error
================
*/
void SV_Shutdown (char *finalmsg, qboolean reconnect)
{
    if (svs.clients)
        SV_FinalMessage (finalmsg, reconnect);

    Master_Shutdown ();
    SV_ShutdownGameProgs ();

    // free current level
    if (sv.demofile)
        fclose (sv.demofile);
    memset (&sv, 0, sizeof(sv));
    Com_SetServerState (sv.state);

    // free server static data
    if (svs.clients)
        Z_Free (svs.clients);
    if (svs.client_entities)
        Z_Free (svs.client_entities);
    if (svs.demofile)
        fclose (svs.demofile);
    memset (&svs, 0, sizeof(svs));
}

void SV_FlushRedirect (int sv_redirected, char *outputbuf)
{
    if (sv_redirected == RD_PACKET)
    {
        Netchan_OutOfBandPrint (NS_SERVER, net_from, "print\n%s", outputbuf);
    }
    else if (sv_redirected == RD_CLIENT)
    {
        MSG_WriteByte (&sv_client->netchan.message, svc_print);
        MSG_WriteByte (&sv_client->netchan.message, PRINT_HIGH);
        MSG_WriteString (&sv_client->netchan.message, outputbuf);
    }
}

int NumForEdict( struct edict_s* e )
{
    return (
        (byte *)(e)-(byte *)ge.edicts
        ) / ge.edict_size;
}


//https://stackoverflow.com/questions/25789895/returned-pointers-value-unexpectedly-changed-after-a-function-call
// Edict should be on the heap.
edict_t* EdictNum( int n )
{
    long unsigned int add;
    if( ge.edict_size == 0 )
    {
        return NULL;
    }

    edict_t* result = &ge.edicts[n];
    add = result;
    add = add / 8;
    return result;
    // return (
    //     (edict_s *)
    //     (
    //         (byte *)ge.edicts + ge.edict_size*(n)
    //     )
    //     );
}