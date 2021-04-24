#ifndef ____SHARED_DEFINES_H__
#define ____SHARED_DEFINES_H__

constexpr char* BASEDIRNAME = "baseq2";

#include <string>

using String = std::string;

typedef unsigned char byte;

#define MAXPRINTMSG 16384

#define WINDOW_CLASS_NAME "Quake 2"

#define ERR_FATAL 0       // exit the entire game with a popup window
#define ERR_DROP 1        // print to console and disconnect from game
#define ERR_DISCONNECT 2  // don't kill server
#define ERR_QUIT 3        // not an error, just a normal exit

#define PRINT_ALL 0
#define PRINT_DEVELOPER 1  // only print when "developer 1"
#define PRINT_ALERT 2

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
    svc_sound,           // <see code>
    svc_print,           // [byte] id [string] null terminated string
    svc_stufftext = 11,  // [string] stuffed into client's console buffer,
                         // should be \n terminated
    svc_serverdata,      // [long] protocol ...
    svc_configstring,    // [short] [string]
    svc_spawnbaseline,
    svc_centerprint,          // [string] to put in center of the screen
    svc_download,             // [short] size [size bytes]
    svc_playerinfo,           // variable
    svc_packetentities,       // [...]
    svc_deltapacketentities,  // [...]
    svc_frame
};

#define NUMVERTEXNORMALS 162

#define MAX_QPATH 64  // max length of a quake game pathname

using vec_t = float;
using vec3_t = vec_t[3];
using vec5_t = vec_t[5];

typedef int fixed4_t;
typedef int fixed8_t;
typedef int fixed16_t;

typedef struct
{
    vec3_t position;
} mvertex_t;

// !!! if this is changed, it must be changed in asm_draw.h too !!!
typedef struct
{
    unsigned short v[2];
    unsigned int cachededgeoffset;
} medge_t;

#define MIPLEVELS 4
#define MAXLIGHTMAPS 4

#define VERTEXSIZE 7

typedef enum
{
    mod_bad,
    mod_brush,
    mod_sprite,
    mod_alias
} modtype_t;

// 3dstudio environment map names
extern char* suf[6];

#define MAX_ENT_CLUSTERS 16

//
// key / value info strings
//
#define MAX_INFO_KEY 64
#define MAX_INFO_VALUE 64
#define MAX_INFO_STRING 512

typedef enum
{
    WEAPON_READY,
    WEAPON_ACTIVATING,
    WEAPON_DROPPING,
    WEAPON_FIRING
} weaponstate_t;

typedef enum
{
    DAMAGE_NO,
    DAMAGE_YES,  // will take damage if hit
    DAMAGE_AIM   // auto targeting recognizes this
} damage_t;

#define MAX_STATS 32

// pmove_state is the information necessary for client side movement
// prediction
enum class pmtype_t : short
{
    // can accelerate and turn
    PM_NORMAL,
    PM_SPECTATOR,
    // no acceleration or turning
    PM_DEAD,
    PM_GIB,  // different bounding box
    PM_FREEZE
};

//
// per-level limits
//
#define MAX_CLIENTS 256  // absolute limit
#define MAX_EDICTS 1024  // must change protocol to increase more
#define MAX_LIGHTSTYLES 256
#define MAX_MODELS 256  // these are sent over the net as bytes
#define MAX_SOUNDS 256  // so they cannot be blindly increased
#define MAX_IMAGES 256
#define MAX_ITEMS 256
#define MAX_GENERAL ( MAX_CLIENTS * 2 )  // general config strings

struct packfile
{
    std::string name;
    int filepos = 0, filelen = 0;
};

// nothing outside the Cvar_*() functions should modify these fields!
struct cvar
{
    std::string name;
    std::string string;
    std::string latched_string;  // for CVAR_LATCH vars
    int flags;
    bool modified;  // set each time the cvar is changed
    float value;
    cvar* next;
};

char* toChar( const std::string& someString );

//
// button bits
//
#define BUTTON_ATTACK 1
#define BUTTON_USE 2
#define BUTTON_ANY 128  // any key whatsoever

#define MAXTOUCH 32

#define TEXNUM_LIGHTMAPS 1024
#define TEXNUM_SCRAPS 1152
#define TEXNUM_IMAGES 1153

#define MAX_GLTEXTURES 1024

#endif  // ____SHARED_DEFINES_H__
