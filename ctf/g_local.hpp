#ifndef ____CTF_G_LOCAL_H__
#define ____CTF_G_LOCAL_H__

/*
Copyright (C) 1997-2001 Id Software, Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/
// g_local.h -- local definitions for game module

#include "q_shared.hpp"

// define GAME_INCLUDE so that game.h does not define the
// short, server-visible gclient_t and struct edict_s structures,
// because we define the full size ones in this file
#define    GAME_INCLUDE
#include "game.hpp"

//ZOID
#include "p_menu.hpp"
//ZOID

// the "gameversion" client command will print this plus compile date
#define    GAMEVERSION    "baseq2"

//==================================================================

// view pitching times
#define DAMAGE_TIME        0.5
#define    FALL_TIME        0.3


// edict->spawnflags
// these are set with checkboxes on each entity in the map editor
#define    SPAWNFLAG_NOT_EASY            0x00000100
#define    SPAWNFLAG_NOT_MEDIUM        0x00000200
#define    SPAWNFLAG_NOT_HARD            0x00000400
#define    SPAWNFLAG_NOT_DEATHMATCH    0x00000800
#define    SPAWNFLAG_NOT_COOP            0x00001000

// edict->flags
#define    FL_FLY                    0x00000001
#define    FL_SWIM                    0x00000002    // implied immunity to drowining
#define FL_IMMUNE_LASER            0x00000004
#define    FL_INWATER                0x00000008
#define    FL_GODMODE                0x00000010
#define    FL_NOTARGET                0x00000020
#define FL_IMMUNE_SLIME            0x00000040
#define FL_IMMUNE_LAVA            0x00000080
#define    FL_PARTIALGROUND        0x00000100    // not all corners are valid
#define    FL_WATERJUMP            0x00000200    // player jumping out of water
#define    FL_TEAMSLAVE            0x00000400    // not the first on the team
#define FL_NO_KNOCKBACK            0x00000800
#define FL_POWER_ARMOR            0x00001000    // power armor (if any) is active
#define FL_RESPAWN                0x80000000    // used for item respawning


#define    FRAMETIME        0.1

// memory tags to allow dynamic memory to be cleaned up
#define    TAG_GAME    765        // clear when unloading the dll
#define    TAG_LEVEL    766        // clear when loading a new level


#define MELEE_DISTANCE    80

#define BODY_QUEUE_SIZE        8

typedef enum
{
    AMMO_BULLETS,
    AMMO_SHELLS,
    AMMO_ROCKETS,
    AMMO_GRENADES,
    AMMO_CELLS,
    AMMO_SLUGS
} ammo_t;


//deadflag
#define DEAD_NO                    0
#define DEAD_DYING                1
#define DEAD_DEAD                2
#define DEAD_RESPAWNABLE        3

//range
#define RANGE_MELEE                0
#define RANGE_NEAR                1
#define RANGE_MID                2
#define RANGE_FAR                3

//gib types
#define GIB_ORGANIC                0
#define GIB_METALLIC            1

//monster ai flags
#define AI_STAND_GROUND            0x00000001
#define AI_TEMP_STAND_GROUND    0x00000002
#define AI_SOUND_TARGET            0x00000004
#define AI_LOST_SIGHT            0x00000008
#define AI_PURSUIT_LAST_SEEN    0x00000010
#define AI_PURSUE_NEXT            0x00000020
#define AI_PURSUE_TEMP            0x00000040
#define AI_HOLD_FRAME            0x00000080
#define AI_GOOD_GUY                0x00000100
#define AI_BRUTAL                0x00000200
#define AI_NOSTEP                0x00000400
#define AI_DUCKED                0x00000800
#define AI_COMBAT_POINT            0x00001000
#define AI_MEDIC                0x00002000
#define AI_RESURRECTING            0x00004000

//monster attack state
#define AS_STRAIGHT                1
#define AS_SLIDING                2
#define    AS_MELEE                3
#define    AS_MISSILE                4

// armor types
#define ARMOR_NONE                0
#define ARMOR_JACKET            1
#define ARMOR_COMBAT            2
#define ARMOR_BODY                3
#define ARMOR_SHARD                4

// power armor types
#define POWER_ARMOR_NONE        0
#define POWER_ARMOR_SCREEN        1
#define POWER_ARMOR_SHIELD        2

// handedness values
#define RIGHT_HANDED            0
#define LEFT_HANDED                1
#define CENTER_HANDED            2


// game.serverflags values
#define SFL_CROSS_TRIGGER_1        0x00000001
#define SFL_CROSS_TRIGGER_2        0x00000002
#define SFL_CROSS_TRIGGER_3        0x00000004
#define SFL_CROSS_TRIGGER_4        0x00000008
#define SFL_CROSS_TRIGGER_5        0x00000010
#define SFL_CROSS_TRIGGER_6        0x00000020
#define SFL_CROSS_TRIGGER_7        0x00000040
#define SFL_CROSS_TRIGGER_8        0x00000080
#define SFL_CROSS_TRIGGER_MASK    0x000000ff


// noise types for PlayerNoise
#define PNOISE_SELF                0
#define PNOISE_WEAPON            1
#define PNOISE_IMPACT            2


// edict->movetype values
typedef enum
{
MOVETYPE_NONE,            // never moves
MOVETYPE_NOCLIP,        // origin and angles change with no interaction
MOVETYPE_PUSH,            // no clip to world, push on box contact
MOVETYPE_STOP,            // no clip to world, stops on box contact

MOVETYPE_WALK,            // gravity
MOVETYPE_STEP,            // gravity, special edge handling
MOVETYPE_FLY,
MOVETYPE_TOSS,            // gravity
MOVETYPE_FLYMISSILE,    // extra size to monsters
MOVETYPE_BOUNCE
} movetype_t;



typedef struct
{
    int        base_count;
    int        max_count;
    float    normal_protection;
    float    energy_protection;
    int        armor;
} gitem_armor_t;


// gitem_t->flags
#define    IT_WEAPON        1        // use makes active weapon
#define    IT_AMMO            2
#define IT_ARMOR        4
#define IT_STAY_COOP    8
#define IT_KEY            16
#define IT_POWERUP        32
//ZOID
#define IT_TECH            64
//ZOID

// gitem_t->weapmodel for weapons indicates model index
#define WEAP_BLASTER            1
#define WEAP_SHOTGUN            2
#define WEAP_SUPERSHOTGUN        3
#define WEAP_MACHINEGUN            4
#define WEAP_CHAINGUN            5
#define WEAP_GRENADES            6
#define WEAP_GRENADELAUNCHER    7
#define WEAP_ROCKETLAUNCHER        8
#define WEAP_HYPERBLASTER        9
#define WEAP_RAILGUN            10
#define WEAP_BFG                11
#define WEAP_GRAPPLE            12

typedef struct gitem_s
{
    char        *classname;    // spawning name
    bool    (*pickup)(struct edict_s *ent, struct edict_s *other);
    void        (*use)(struct edict_s *ent, struct gitem_s *item);
    void        (*drop)(struct edict_s *ent, struct gitem_s *item);
    void        (*weaponthink)(struct edict_s *ent);
    char        *pickup_sound;
    char        *world_model;
    int            world_model_flags;
    char        *view_model;

    // client side info
    char        *icon;
    char        *pickup_name;    // for printing on pickup
    int            count_width;        // number of digits to display by icon

    int            quantity;        // for ammo how much, for weapons how much is used per shot
    char        *ammo;            // for weapons
    int            flags;            // IT_* flags

    int            weapmodel;        // weapon model index (for weapons)

    void        *info;
    int            tag;

    char        *precaches;        // string of all models, sounds, and images this item will use
} gitem_t;



//
// this structure is left intact through an entire game
// it should be initialized at dll load time, and read/written to
// the server.ssv file for savegames
//
typedef struct
{
    char        helpmessage1[512];
    char        helpmessage2[512];
    int            helpchanged;    // flash F1 icon if non 0, play sound
                                // and increment only if 1, 2, or 3

    gclient_t    *clients;        // [maxclients]

    // can't store spawnpoint in level, because
    // it would get overwritten by the savegame restore
    char        spawnpoint[512];    // needed for coop respawns

    // store latched cvars here that we want to get at often
    int            maxclients;
    int            maxentities;

    // cross level triggers
    int            serverflags;

    // items
    int            num_items;

    bool    autosaved;
} game_locals_t;


//
// this structure is cleared as each map is entered
// it is read/written to the level.sav file for savegames
//
typedef struct
{
    int            framenum;
    float        time;

    char        level_name[MAX_QPATH];    // the descriptive name (Outer Base, etc)
    char        mapname[MAX_QPATH];        // the server name (base1, etc)
    char        nextmap[MAX_QPATH];        // go here when fraglimit is hit
    char        forcemap[MAX_QPATH];    // go here

    // intermission state
    float        intermissiontime;        // time the intermission was started
    char        *changemap;
    int            exitintermission;
    vec3_t        intermission_origin;
    vec3_t        intermission_angle;

    struct edict_s        *sight_client;    // changed once each frame for coop games

    struct edict_s        *sight_entity;
    int            sight_entity_framenum;
    struct edict_s        *sound_entity;
    int            sound_entity_framenum;
    struct edict_s        *sound2_entity;
    int            sound2_entity_framenum;

    int            pic_health;

    int            total_secrets;
    int            found_secrets;

    int            total_goals;
    int            found_goals;

    int            total_monsters;
    int            killed_monsters;

    struct edict_s        *current_entity;    // entity running from G_RunFrame
    int            body_que;            // dead bodies

    int            power_cubes;        // ugly necessity for coop
} level_locals_t;


// spawn_temp_t is only used to hold entity field values that
// can be set from the editor, but aren't actualy present
// in struct edict_s during gameplay
typedef struct
{
    // world vars
    char        *sky;
    float        skyrotate;
    vec3_t        skyaxis;
    char        *nextmap;

    int            lip;
    int            distance;
    int            height;
    char        *noise;
    float        pausetime;
    char        *item;
    char        *gravity;

    float        minyaw;
    float        maxyaw;
    float        minpitch;
    float        maxpitch;
} spawn_temp_t;


typedef struct
{
    // fixed data
    vec3_t        start_origin;
    vec3_t        start_angles;
    vec3_t        end_origin;
    vec3_t        end_angles;

    int            sound_start;
    int            sound_middle;
    int            sound_end;

    float        accel;
    float        speed;
    float        decel;
    float        distance;

    float        wait;

    // state data
    int            state;
    vec3_t        dir;
    float        current_speed;
    float        move_speed;
    float        next_speed;
    float        remaining_distance;
    float        decel_distance;
    void        (*endfunc)(struct edict_s *);
} moveinfo_t;


typedef struct
{
    void    (*aifunc)(struct edict_s *self, float dist);
    float    dist;
    void    (*thinkfunc)(struct edict_s *self);
} mframe_t;

typedef struct
{
    int            firstframe;
    int            lastframe;
    mframe_t    *frame;
    void        (*endfunc)(struct edict_s *self);
} mmove_t;

typedef struct
{
    mmove_t        *currentmove;
    int            aiflags;
    int            nextframe;
    float        scale;

    void        (*stand)(struct edict_s *self);
    void        (*idle)(struct edict_s *self);
    void        (*search)(struct edict_s *self);
    void        (*walk)(struct edict_s *self);
    void        (*run)(struct edict_s *self);
    void        (*dodge)(struct edict_s *self, struct edict_s *other, float eta);
    void        (*attack)(struct edict_s *self);
    void        (*melee)(struct edict_s *self);
    void        (*sight)(struct edict_s *self, struct edict_s *other);
    bool    (*checkattack)(struct edict_s *self);

    float        pausetime;
    float        attack_finished;

    vec3_t        saved_goal;
    float        search_time;
    float        trail_time;
    vec3_t        last_sighting;
    int            attack_state;
    int            lefty;
    float        idle_time;
    int            linkcount;

    int            power_armor_type;
    int            power_armor_power;
} monsterinfo_t;



extern    game_locals_t    game;
extern    level_locals_t    level;
extern    game_import_t    gi;
extern    game_export_t    globals;
extern    spawn_temp_t    st;

extern    int    sm_meat_index;
extern    int    snd_fry;

extern    int    jacket_armor_index;
extern    int    combat_armor_index;
extern    int    body_armor_index;


// means of death
#define MOD_UNKNOWN            0
#define MOD_BLASTER            1
#define MOD_SHOTGUN            2
#define MOD_SSHOTGUN        3
#define MOD_MACHINEGUN        4
#define MOD_CHAINGUN        5
#define MOD_GRENADE            6
#define MOD_G_SPLASH        7
#define MOD_ROCKET            8
#define MOD_R_SPLASH        9
#define MOD_HYPERBLASTER    10
#define MOD_RAILGUN            11
#define MOD_BFG_LASER        12
#define MOD_BFG_BLAST        13
#define MOD_BFG_EFFECT        14
#define MOD_HANDGRENADE        15
#define MOD_HG_SPLASH        16
#define MOD_WATER            17
#define MOD_SLIME            18
#define MOD_LAVA            19
#define MOD_CRUSH            20
#define MOD_TELEFRAG        21
#define MOD_FALLING            22
#define MOD_SUICIDE            23
#define MOD_HELD_GRENADE    24
#define MOD_EXPLOSIVE        25
#define MOD_BARREL            26
#define MOD_BOMB            27
#define MOD_EXIT            28
#define MOD_SPLASH            29
#define MOD_TARGET_LASER    30
#define MOD_TRIGGER_HURT    31
#define MOD_HIT                32
#define MOD_TARGET_BLASTER    33
#define MOD_GRAPPLE            34
#define MOD_FRIENDLY_FIRE    0x8000000

extern    int    meansOfDeath;


extern    struct edict_s            *g_edicts;

#define    FOFS(x) (int)&(((struct edict_s *)0)->x)
#define    STOFS(x) (int)&(((spawn_temp_t *)0)->x)
#define    LLOFS(x) (int)&(((level_locals_t *)0)->x)
#define    CLOFS(x) (int)&(((gclient_t *)0)->x)

#define random()    ((rand () & 0x7fff) / ((float)0x7fff))
#define crandom()    (2.0 * (random() - 0.5))

extern    cvar_t    *maxentities;
extern    cvar_t    *deathmatch;
extern    cvar_t    *coop;
extern    cvar_t    *dmflags;
extern    cvar_t    *skill;
extern    cvar_t    *fraglimit;
extern    cvar_t    *timelimit;
//ZOID
extern    cvar_t    *capturelimit;
extern    cvar_t    *instantweap;
//ZOID
extern    cvar_t    *password;
extern    cvar_t    *g_select_empty;
extern    cvar_t    *dedicated;

extern    cvar_t    *sv_gravity;
extern    cvar_t    *sv_maxvelocity;

extern    cvar_t    *gun_x, *gun_y, *gun_z;
extern    cvar_t    *sv_rollspeed;
extern    cvar_t    *sv_rollangle;

extern    cvar_t    *run_pitch;
extern    cvar_t    *run_roll;
extern    cvar_t    *bob_up;
extern    cvar_t    *bob_pitch;
extern    cvar_t    *bob_roll;

extern    cvar_t    *sv_cheats;
extern    cvar_t    *maxclients;

extern    cvar_t    *flood_msgs;
extern    cvar_t    *flood_persecond;
extern    cvar_t    *flood_waitdelay;

extern    cvar_t    *sv_maplist;

//ZOID
extern    bool    is_quad;
//ZOID

#define world    (&g_edicts[0])

// item spawnflags
#define ITEM_TRIGGER_SPAWN        0x00000001
#define ITEM_NO_TOUCH            0x00000002
// 6 bits reserved for editor flags
// 8 bits used as power cube id bits for coop games
#define DROPPED_ITEM            0x00010000
#define    DROPPED_PLAYER_ITEM        0x00020000
#define ITEM_TARGETS_USED        0x00040000

//
// fields are needed for spawning from the entity string
// and saving / loading games
//
#define FFL_SPAWNTEMP        1

typedef enum {
    F_INT,
    F_FLOAT,
    F_LSTRING,            // string on disk, pointer in memory, TAG_LEVEL
    F_GSTRING,            // string on disk, pointer in memory, TAG_GAME
    F_VECTOR,
    F_ANGLEHACK,
    F_EDICT,            // index on disk, pointer in memory
    F_ITEM,                // index on disk, pointer in memory
    F_CLIENT,            // index on disk, pointer in memory
    F_IGNORE
} fieldtype_t;

typedef struct
{
    char    *name;
    int        ofs;
    fieldtype_t    type;
    int        flags;
} field_t;


extern    field_t fields[];
extern    gitem_t    itemlist[];


//
// g_cmds.c
//
bool CheckFlood(struct edict_s *ent);
void Cmd_Help_f (struct edict_s *ent);
void Cmd_Score_f (struct edict_s *ent);

//
// g_items.c
//
void PrecacheItem (gitem_t *it);
void InitItems (void);
void SetItemNames (void);
gitem_t    *FindItem (char *pickup_name);
gitem_t    *FindItemByClassname (char *classname);
#define    ITEM_INDEX(x) ((x)-itemlist)
struct edict_s *Drop_Item (struct edict_s *ent, gitem_t *item);
void SetRespawn (struct edict_s *ent, float delay);
void ChangeWeapon (struct edict_s *ent);
void SpawnItem (struct edict_s *ent, gitem_t *item);
void Think_Weapon (struct edict_s *ent);
int ArmorIndex (struct edict_s *ent);
int PowerArmorType (struct edict_s *ent);
gitem_t    *GetItemByIndex (int index);
bool Add_Ammo (struct edict_s *ent, gitem_t *item, int count);
void Touch_Item (struct edict_s *ent, struct edict_s *other, plane_t *plane, csurface_t *surf);

//
// g_utils.c
//
bool    KillBox (struct edict_s *ent);
void    G_ProjectSource (vec3_t point, vec3_t distance, vec3_t forward, vec3_t right, vec3_t result);
struct edict_s *G_Find (struct edict_s *from, int fieldofs, char *match);
struct edict_s *findradius (struct edict_s *from, vec3_t org, float rad);
struct edict_s *G_PickTarget (char *targetname);
void    G_UseTargets (struct edict_s *ent, struct edict_s *activator);
void    G_SetMovedir (vec3_t angles, vec3_t movedir);

void    G_InitEdict (struct edict_s *e);
struct edict_s    *G_Spawn (void);
void    G_FreeEdict (struct edict_s *e);

void    G_TouchTriggers (struct edict_s *ent);
void    G_TouchSolids (struct edict_s *ent);

char    *G_CopyString (char *in);

float    *tv (float x, float y, float z);
char    *vtos (vec3_t v);

float vectoyaw (vec3_t vec);
void vectoangles (vec3_t vec, vec3_t angles);

//
// g_combat.c
//
bool OnSameTeam (struct edict_s *ent1, struct edict_s *ent2);
bool CanDamage (struct edict_s *targ, struct edict_s *inflictor);
bool CheckTeamDamage (struct edict_s *targ, struct edict_s *attacker);
void T_Damage (struct edict_s *targ, struct edict_s *inflictor, struct edict_s *attacker, vec3_t dir, vec3_t point, vec3_t normal, int damage, int knockback, int dflags, int mod);
void T_RadiusDamage (struct edict_s *inflictor, struct edict_s *attacker, float damage, struct edict_s *ignore, float radius, int mod);

// damage flags
#define DAMAGE_RADIUS            0x00000001    // damage was indirect
#define DAMAGE_NO_ARMOR            0x00000002    // armour does not protect from this damage
#define DAMAGE_ENERGY            0x00000004    // damage is from an energy based weapon
#define DAMAGE_NO_KNOCKBACK        0x00000008    // do not affect velocity, just view angles
#define DAMAGE_BULLET            0x00000010  // damage is from a bullet (used for ricochets)
#define DAMAGE_NO_PROTECTION    0x00000020  // armor, shields, invulnerability, and godmode have no effect

#define DEFAULT_BULLET_HSPREAD    300
#define DEFAULT_BULLET_VSPREAD    500
#define DEFAULT_SHOTGUN_HSPREAD    1000
#define DEFAULT_SHOTGUN_VSPREAD    500
#define DEFAULT_DEATHMATCH_SHOTGUN_COUNT    12
#define DEFAULT_SHOTGUN_COUNT    12
#define DEFAULT_SSHOTGUN_COUNT    20

//
// g_monster.c
//
void monster_fire_bullet (struct edict_s *self, vec3_t start, vec3_t dir, int damage, int kick, int hspread, int vspread, int flashtype);
void monster_fire_shotgun (struct edict_s *self, vec3_t start, vec3_t aimdir, int damage, int kick, int hspread, int vspread, int count, int flashtype);
void monster_fire_blaster (struct edict_s *self, vec3_t start, vec3_t dir, int damage, int speed, int flashtype, int effect);
void monster_fire_grenade (struct edict_s *self, vec3_t start, vec3_t aimdir, int damage, int speed, int flashtype);
void monster_fire_rocket (struct edict_s *self, vec3_t start, vec3_t dir, int damage, int speed, int flashtype);
void monster_fire_railgun (struct edict_s *self, vec3_t start, vec3_t aimdir, int damage, int kick, int flashtype);
void monster_fire_bfg (struct edict_s *self, vec3_t start, vec3_t aimdir, int damage, int speed, int kick, float damage_radius, int flashtype);
void M_droptofloor (struct edict_s *ent);
void monster_think (struct edict_s *self);
void walkmonster_start (struct edict_s *self);
void swimmonster_start (struct edict_s *self);
void flymonster_start (struct edict_s *self);
void AttackFinished (struct edict_s *self, float time);
void monster_death_use (struct edict_s *self);
void M_CatagorizePosition (struct edict_s *ent);
bool M_CheckAttack (struct edict_s *self);
void M_FlyCheck (struct edict_s *self);
void M_CheckGround (struct edict_s *ent);

//
// g_misc.c
//
void ThrowHead (struct edict_s *self, char *gibname, int damage, int type);
void ThrowClientHead (struct edict_s *self, int damage);
void ThrowGib (struct edict_s *self, char *gibname, int damage, int type);
void BecomeExplosion1(struct edict_s *self);

//
// g_ai.c
//
void AI_SetSightClient (void);

void ai_stand (struct edict_s *self, float dist);
void ai_move (struct edict_s *self, float dist);
void ai_walk (struct edict_s *self, float dist);
void ai_turn (struct edict_s *self, float dist);
void ai_run (struct edict_s *self, float dist);
void ai_charge (struct edict_s *self, float dist);
int range (struct edict_s *self, struct edict_s *other);

void FoundTarget (struct edict_s *self);
bool infront (struct edict_s *self, struct edict_s *other);
bool visible (struct edict_s *self, struct edict_s *other);
bool FacingIdeal(struct edict_s *self);

//
// g_weapon.c
//
void ThrowDebris (struct edict_s *self, char *modelname, float speed, vec3_t origin);
bool fire_hit (struct edict_s *self, vec3_t aim, int damage, int kick);
void fire_bullet (struct edict_s *self, vec3_t start, vec3_t aimdir, int damage, int kick, int hspread, int vspread, int mod);
void fire_shotgun (struct edict_s *self, vec3_t start, vec3_t aimdir, int damage, int kick, int hspread, int vspread, int count, int mod);
void fire_blaster (struct edict_s *self, vec3_t start, vec3_t aimdir, int damage, int speed, int effect, bool hyper);
void fire_grenade (struct edict_s *self, vec3_t start, vec3_t aimdir, int damage, int speed, float timer, float damage_radius);
void fire_grenade2 (struct edict_s *self, vec3_t start, vec3_t aimdir, int damage, int speed, float timer, float damage_radius, bool held);
void fire_rocket (struct edict_s *self, vec3_t start, vec3_t dir, int damage, int speed, float damage_radius, int radius_damage);
void fire_rail (struct edict_s *self, vec3_t start, vec3_t aimdir, int damage, int kick);
void fire_bfg (struct edict_s *self, vec3_t start, vec3_t dir, int damage, int speed, float damage_radius);

//
// g_ptrail.c
//
void PlayerTrail_Init (void);
void PlayerTrail_Add (vec3_t spot);
void PlayerTrail_New (vec3_t spot);
struct edict_s *PlayerTrail_PickFirst (struct edict_s *self);
struct edict_s *PlayerTrail_PickNext (struct edict_s *self);
struct edict_s    *PlayerTrail_LastSpot (void);


//
// g_client.c
//
void respawn (struct edict_s *ent);
void BeginIntermission (struct edict_s *targ);
void PutClientInServer (struct edict_s *ent);
void InitClientPersistant (gclient_t *client);
void InitClientResp (gclient_t *client);
void InitBodyQue (void);
void ClientBeginServerFrame (struct edict_s *ent);

//
// g_player.c
//
void player_pain (struct edict_s *self, struct edict_s *other, float kick, int damage);
void player_die (struct edict_s *self, struct edict_s *inflictor, struct edict_s *attacker, int damage, vec3_t point);

//
// g_svcmds.c
//
void    ServerCommand (void);

//
// p_view.c
//
void ClientEndServerFrame (struct edict_s *ent);

//
// p_hud.c
//
void MoveClientToIntermission (struct edict_s *client);
void G_SetStats (struct edict_s *ent);
void ValidateSelectedItem (struct edict_s *ent);
void DeathmatchScoreboardMessage (struct edict_s *client, struct edict_s *killer);

//
// g_pweapon.c
//
void PlayerNoise(struct edict_s *who, vec3_t where, int type);
void P_ProjectSource (gclient_t *client, vec3_t point, vec3_t distance, vec3_t forward, vec3_t right, vec3_t result);
void Weapon_Generic (struct edict_s *ent, int FRAME_ACTIVATE_LAST, int FRAME_FIRE_LAST, int FRAME_IDLE_LAST, int FRAME_DEACTIVATE_LAST, int *pause_frames, int *fire_frames, void (*fire)(struct edict_s *ent));

//
// m_move.c
//
bool M_CheckBottom (struct edict_s *ent);
bool M_walkmove (struct edict_s *ent, float yaw, float dist);
void M_MoveToGoal (struct edict_s *ent, float dist);
void M_ChangeYaw (struct edict_s *ent);

//
// g_phys.c
//
void G_RunEntity (struct edict_s *ent);

//
// g_main.c
//
void SaveClientData (void);
void FetchClientEntData (struct edict_s *ent);
void EndDMLevel (void);


//============================================================================

// client_t->anim_priority
#define    ANIM_BASIC        0        // stand / run
#define    ANIM_WAVE        1
#define    ANIM_JUMP        2
#define    ANIM_PAIN        3
#define    ANIM_ATTACK        4
#define    ANIM_DEATH        5
#define    ANIM_REVERSE    6

// this structure is cleared on each PutClientInServer(),
// except for 'client->pers'

//ZOID
#include "g_ctf.hpp"
//ZOID


#endif // ____CTF_G_LOCAL_H__