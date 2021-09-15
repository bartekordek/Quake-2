#ifndef ____SHARED_SHARED_REFIMPORT_H__
#define ____SHARED_SHARED_REFIMPORT_H__


#include "shared/cvar.h"
#include "shared/entity.h"

typedef struct
{
    vec3_t origin;
    vec3_t color;
    float intensity;
} dlight_t;

typedef struct
{
    vec3_t origin;
    int color;
    float alpha;
} particle_t;

typedef struct
{
    float rgb[3];  // 0.0 - 2.0
    float white;   // highest of rgb
} lightstyle_t;




typedef struct
{
    int x, y, width, height;  // in virtual screen coordinates
    float fov_x, fov_y;
    float vieworg[3];
    float viewangles[3];
    float blend[4];  // rgba 0-1 full screen blend
    float time;      // time is uesed to auto animate
    int rdflags;     // RDF_UNDERWATER, etc

    byte* areabits;  // if not NULL, only areas with set bits will be drawn

    lightstyle_t* lightstyles;  // [MAX_LIGHTSTYLES]

    int num_entities;
    entity_t* entities;

    int num_dlights;
    dlight_t* dlights;

    int num_particles;
    particle_t* particles;
} refdef_t;

//
// these are the functions exported by the refresh module
//
typedef struct
{
    // if api_version is different, the dll cannot be used
    int api_version;

    // called when the library is loaded
    qboolean ( *Init )( void* hinstance, void* wndproc );

    // called before the library is unloaded
    void ( *Shutdown )( void );

    // All data that will be used in a level should be
    // registered before rendering any frames to prevent disk hits,
    // but they can still be registered at a later time
    // if necessary.
    //
    // EndRegistration will free any remaining data that wasn't registered.
    // Any model_s or skin_s pointers from before the BeginRegistration
    // are no longer valid after EndRegistration.
    //
    // Skins and images need to be differentiated, because skins
    // are flood filled to eliminate mip map edge errors, and pics have
    // an implicit "pics/" prepended to the name. (a pic name that starts with a
    // slash will not use the "pics/" prefix or the ".pcx" postfix)
    void ( *BeginRegistration )( char* map );
    struct model_s* ( *RegisterModel )( char* name );
    struct image_s* ( *RegisterSkin )( char* name );
    struct image_s* ( *RegisterPic )( char* name );
    void ( *SetSky )( char* name, float rotate, vec3_t axis );
    void ( *EndRegistration )( void );

    void ( *RenderFrame )( refdef_t* fd );

    void ( *DrawGetPicSize )( int* w, int* h,
                              char* name );  // will return 0 0 if not found
    void ( *DrawPic )( int x, int y, char* name );
    void ( *DrawStretchPic )( int x, int y, int w, int h, char* name );
    void ( *DrawChar )( int x, int y, int c );
    void ( *DrawTileClear )( int x, int y, int w, int h, char* name );
    void ( *DrawFill )( int x, int y, int w, int h, int c );
    void ( *DrawFadeScreen )( void );

    // Draw images for cinematic rendering (which can have a different palette).
    // Note that calls
    void ( *DrawStretchRaw )( int x, int y, int w, int h, int cols, int rows,
                              byte* data );

    /*
    ** video mode and refresh state management entry points
    */
    void ( *CinematicSetPalette )(
        const unsigned char* palette );  // NULL = game palette
    void ( *BeginFrame )( float camera_separation );
    void ( *EndFrame )( void );

    void ( *AppActivate )( qboolean activate );

} refexport_t;

//
// these are the functions imported by the refresh module
//
typedef struct
{
    void ( *Sys_Error )( int err_level, char* str, ... );

    void ( *Cmd_AddCommand )( char* name, void ( *cmd )( void ) );
    void ( *Cmd_RemoveCommand )( char* name );
    int ( *Cmd_Argc )( void );
    char* ( *Cmd_Argv )( int i );
    void ( *Cmd_ExecuteText )( int exec_when, char* text );

    void ( *Con_Printf )( int print_level, char* str, ... );

    // files will be memory mapped read only
    // the returned buffer may be part of a larger pak file,
    // or a discrete file from anywhere in the quake search path
    // a -1 return means the file does not exist
    // NULL can be passed for buf to just determine existance
    int ( *FS_LoadFile )( char* name, void** buf );
    void ( *FS_FreeFile )( void* buf );

    // gamedir will be the current directory that generated
    // files should be stored to, ie: "f:\quake\id1"
    char* ( *FS_Gamedir )( void );

    cvar_t* ( *Cvar_Get )( char* name, char* value, int flags );
    cvar_t* ( *Cvar_Set )( char* name, char* value );
    void ( *Cvar_SetValue )( char* name, float value );

    qboolean ( *Vid_GetModeInfo )( int* width, int* height, int mode );
    void ( *Vid_MenuInit )( void );
    void ( *Vid_NewWindow )( int width, int height );
} refimport_t;

#endif // ____SHARED_SHARED_REFIMPORT_H__