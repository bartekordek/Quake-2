#ifndef ____SHARED_DEFINES_H__
#define ____SHARED_DEFINES_H__



typedef unsigned char         byte;
typedef enum {false, true}    qboolean;

#define    MAXPRINTMSG    4096

#define    WINDOW_CLASS_NAME    "Quake 2"

#define    ERR_FATAL            0        // exit the entire game with a popup window
#define    ERR_DROP            1        // print to console and disconnect from game
#define    ERR_DISCONNECT        2        // don't kill server
#define    ERR_QUIT            3        // not an error, just a normal exit

#define    PRINT_ALL            0
#define PRINT_DEVELOPER        1        // only print when "developer 1"
#define PRINT_ALERT            2

//==================
// the svc_strings[] array in cl_parse.c should mirror this
//==================

// protocol bytes that can be directly added to messages
//
// server to client
//
enum svc_ops_e
{
    svc_bad,

    // these ops are known to the game dll
    svc_muzzleflash = 1,
    svc_muzzleflash2 = 2,
    svc_temp_entity = 3,
    svc_layout = 4,
    svc_inventory = 5,

    // the rest are private to the client and server
    svc_nop,
    svc_disconnect,
    svc_reconnect,
    svc_sound,                    // <see code>
    svc_print,                    // [byte] id [string] null terminated string
    svc_stufftext = 11,                // [string] stuffed into client's console buffer, should be \n terminated
    svc_serverdata,                // [long] protocol ...
    svc_configstring,            // [short] [string]
    svc_spawnbaseline,
    svc_centerprint,            // [string] to put in center of the screen
    svc_download,                // [short] size [size bytes]
    svc_playerinfo,                // variable
    svc_packetentities,            // [...]
    svc_deltapacketentities,    // [...]
    svc_frame
};

#define NUMVERTEXNORMALS    162

#define    MAX_QPATH            64        // max length of a quake game pathname

typedef float vec_t;
typedef vec_t vec3_t[3];
typedef vec_t vec5_t[5];

typedef    int    fixed4_t;
typedef    int    fixed8_t;
typedef    int    fixed16_t;

typedef struct
{
    vec3_t        position;
} mvertex_t;

// !!! if this is changed, it must be changed in asm_draw.h too !!!
typedef struct
{
    unsigned short    v[2];
    unsigned int    cachededgeoffset;
} medge_t;

#define    MIPLEVELS    4
#define    MAXLIGHTMAPS    4

#define    VERTEXSIZE    7

typedef enum {mod_bad, mod_brush, mod_sprite, mod_alias } modtype_t;

// 3dstudio environment map names
extern char *suf[6];

#endif // ____SHARED_DEFINES_H__
