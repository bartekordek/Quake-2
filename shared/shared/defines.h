#ifndef ____SHARED_SHARED_DEFINES_H__
#define ____SHARED_SHARED_DEFINES_H__

typedef unsigned char byte;
typedef enum
{
    false,
    true
} qboolean;

#define MAX_QPATH 64    // max length of a quake game pathname
#define MAX_OSPATH 128  // max length of a filesystem pathname

#define MAXLIGHTMAPS 4

#define MAX_TRIANGLES 4096
#define MAX_VERTS 2048
#define MAX_FRAMES 512
#define MAX_MD2SKINS 32
#define MAX_SKINNAME 64

#define PRINT_ALL 0
#define PRINT_DEVELOPER 1  // only print when "developer 1"
#define PRINT_ALERT 2

// game print flags
#define PRINT_LOW 0     // pickup messages
#define PRINT_MEDIUM 1  // death messages
#define PRINT_HIGH 2    // critical messages
#define PRINT_CHAT 3    // chat messages

#define ERR_FATAL 0       // exit the entire game with a popup window
#define ERR_DROP 1        // print to console and disconnect from game
#define ERR_DISCONNECT 2  // don't kill server

// contents flags are seperate bits
// a given brush can contribute multiple content bits
// multiple brushes can be in a single leaf

// these definitions also need to be in q_shared.h!

// lower bits are stronger, and will eat weaker brushes completely
#define CONTENTS_SOLID 1   // an eye is never valid in a solid
#define CONTENTS_WINDOW 2  // translucent, but not watery
#define CONTENTS_AUX 4
#define CONTENTS_LAVA 8
#define CONTENTS_SLIME 16
#define CONTENTS_WATER 32
#define CONTENTS_MIST 64
#define LAST_VISIBLE_CONTENTS 64

// remaining contents are non-visible, and don't eat brushes

#define CONTENTS_AREAPORTAL 0x8000

#define CONTENTS_PLAYERCLIP 0x10000
#define CONTENTS_MONSTERCLIP 0x20000

// currents can be added to any other contents, and may be mixed
#define CONTENTS_CURRENT_0 0x40000
#define CONTENTS_CURRENT_90 0x80000
#define CONTENTS_CURRENT_180 0x100000
#define CONTENTS_CURRENT_270 0x200000
#define CONTENTS_CURRENT_UP 0x400000
#define CONTENTS_CURRENT_DOWN 0x800000

#define CONTENTS_ORIGIN 0x1000000  // removed before bsping an entity

#define CONTENTS_MONSTER 0x2000000  // should never be on a brush, only in game
#define CONTENTS_DEADMONSTER 0x4000000
#define CONTENTS_DETAIL 0x8000000        // brushes to be added after vis leafs
#define CONTENTS_TRANSLUCENT 0x10000000  // auto set if any surface has trans
#define CONTENTS_LADDER 0x20000000

#define SURF_LIGHT 0x1  // value will hold the light strength

#define SURF_SLICK 0x2  // effects game physics

#define SURF_SKY 0x4   // don't draw, but add to skybox
#define SURF_WARP 0x8  // turbulent water warp
#define SURF_TRANS33 0x10
#define SURF_TRANS66 0x20
#define SURF_FLOWING 0x40  // scroll towards angle
#define SURF_NODRAW 0x80   // don't bother referencing the texture

// entity_state_t->renderfx flags
#define RF_MINLIGHT 1     // allways have some light (viewmodel)
#define RF_VIEWERMODEL 2  // don't draw through eyes, only mirrors
#define RF_WEAPONMODEL 4  // only draw through eyes
#define RF_FULLBRIGHT 8   // allways draw full intensity
#define RF_DEPTHHACK 16   // for view weapon Z crunching
#define RF_TRANSLUCENT 32
#define RF_FRAMELERP 64
#define RF_BEAM 128
#define RF_CUSTOMSKIN 256  // skin is an index in image_precache
#define RF_GLOW 512        // pulse lighting for bonus items
#define RF_SHELL_RED 1024
#define RF_SHELL_GREEN 2048
#define RF_SHELL_BLUE 4096

// ROGUE
#define RF_IR_VISIBLE 0x00008000    // 32768
#define RF_SHELL_DOUBLE 0x00010000  // 65536
#define RF_SHELL_HALF_DAM 0x00020000
#define RF_USE_DISGUISE 0x00040000
// ROGUE

#define MAX_DLIGHTS 32
#define MAX_ENTITIES 128
#define MAX_PARTICLES 4096
#define MAX_LIGHTSTYLES 256

#define POWERSUIT_SCALE 4.0F

#define SHELL_RED_COLOR 0xF2
#define SHELL_GREEN_COLOR 0xD0
#define SHELL_BLUE_COLOR 0xF3

#define SHELL_RG_COLOR 0xDC
//#define SHELL_RB_COLOR		0x86
#define SHELL_RB_COLOR 0x68
#define SHELL_BG_COLOR 0x78

// ROGUE
#define SHELL_DOUBLE_COLOR 0xDF  // 223
#define SHELL_HALF_DAM_COLOR 0x90
#define SHELL_CYAN_COLOR 0x72
// ROGUE

#define SHELL_WHITE_COLOR 0xD7

#define ENTITY_FLAGS 68

#define API_VERSION 3

/*
==============================================================

MATHLIB

==============================================================
*/

typedef float vec_t;
typedef vec_t vec3_t[3];
typedef vec_t vec5_t[5];

typedef int fixed4_t;
typedef int fixed8_t;
typedef int fixed16_t;

#ifndef M_PI
#define M_PI 3.14159265358979323846  // matches value in gcc v2 math.h
#endif

extern vec3_t vec3_origin;

#endif  // ____SHARED_SHARED_DEFINES_H__