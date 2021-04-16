#ifndef ____GAME_G_LOCAL_H__
#define ____GAME_G_LOCAL_H__

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
#include "shared/edict.hpp"
#include <string>

// define GAME_INCLUDE so that game.h does not define the
// short, server-visible gclient_t and edict structures,
// because we define the full size ones in this file
#define GAME_INCLUDE
#include "game.hpp"

// the "gameversion" client command will print this plus compile date
#define GAMEVERSION "baseq2"

//==================================================================

// view pitching times
#define DAMAGE_TIME 0.5
#define FALL_TIME 0.3


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



typedef struct gclient gclient_t;

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


// gitem->flags
#define    IT_WEAPON        1        // use makes active weapon
#define    IT_AMMO            2
#define IT_ARMOR        4
#define IT_STAY_COOP    8
#define IT_KEY            16
#define IT_POWERUP        32

// gitem->weapmodel for weapons indicates model index
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

    gclient_t* clients;        // [maxclients]

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

    // intermission state
    float        intermissiontime;        // time the intermission was started
    char    * changemap;
    int            exitintermission;
    vec3_t        intermission_origin;
    vec3_t        intermission_angle;

    edict    * sight_client;    // changed once each frame for coop games

    edict    * sight_entity;
    int            sight_entity_framenum;
    edict    * sound_entity;
    int            sound_entity_framenum;
    edict    * sound2_entity;
    int            sound2_entity_framenum;

    int            pic_health;

    int            total_secrets;
    int            found_secrets;

    int            total_goals;
    int            found_goals;

    int            total_monsters;
    int            killed_monsters;

    edict    * current_entity;    // entity running from G_RunFrame
    int            body_que;            // dead bodies

    int            power_cubes;        // ugly necessity for coop
} level_locals_t;


// spawn_temp_t is only used to hold entity field values that
// can be set from the editor, but aren't actualy present
// in edict during gameplay
typedef struct
{
    // world vars
    char    * sky;
    float        skyrotate;
    vec3_t        skyaxis;
    char    * nextmap;

    int            lip;
    int            distance;
    int            height;
    char    * noise;
    float        pausetime;
    char    * item;
    char    * gravity;

    float        minyaw;
    float        maxyaw;
    float        minpitch;
    float        maxpitch;
} spawn_temp_t;

extern    game_locals_t    game;
extern    level_locals_t    level;
game_import    gi;
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
#define MOD_FRIENDLY_FIRE    0x8000000

extern    int    meansOfDeath;


extern    edict        * g_edicts;

#define    FOFS(x) (int)&(((edict *)0)->x)
#define    STOFS(x) (int)&(((spawn_temp_t *)0)->x)
#define    LLOFS(x) (int)&(((level_locals_t *)0)->x)
#define    CLOFS(x) (int)&(((gclient_t *)0)->x)

#define random()    ((rand () & 0x7fff) / ((float)0x7fff))
#define crandom()    (2.0 * (random() - 0.5))

extern    cvar* maxentities;
extern    cvar* deathmatch;
extern    cvar* coop;
extern    cvar* dmflags;
extern    cvar* skill;
extern    cvar* fraglimit;
extern    cvar* timelimit;
extern    cvar* password;
extern    cvar* spectator_password;
extern    cvar* g_select_empty;
extern    cvar* dedicated;

extern    cvar* filterban;

extern    cvar* sv_gravity;
extern    cvar* sv_maxvelocity;

extern    cvar* gun_x, *gun_y, *gun_z;
extern    cvar* sv_rollspeed;
extern    cvar* sv_rollangle;

extern    cvar* run_pitch;
extern    cvar* run_roll;
extern    cvar* bob_up;
extern    cvar* bob_pitch;
extern    cvar* bob_roll;

extern    cvar* sv_cheats;
extern    cvar* maxclients;
extern    cvar* maxspectators;

extern    cvar* flood_msgs;
extern    cvar* flood_persecond;
extern    cvar* flood_waitdelay;

extern    cvar* sv_maplist;

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
#define FFL_NOSPAWN            2

enum class fieldtype_t: short {
F_INT,
F_FLOAT,
F_LSTRING,            // string on disk, pointer in memory, TAG_LEVEL
F_GSTRING,            // string on disk, pointer in memory, TAG_GAME
F_VECTOR,
F_ANGLEHACK,
F_EDICT,            // index on disk, pointer in memory
F_ITEM,                // index on disk, pointer in memory
F_CLIENT,            // index on disk, pointer in memory
F_FUNCTION,
F_MMOVE,
F_IGNORE
} ;

struct field
{
    std::string name;
    int ofs;
    fieldtype_t type;
    int flags;
};


extern field fields[];
extern gitem    itemlist[];


//
// g_cmds.c
//
void Cmd_Help_f (edict *ent);
void Cmd_Score_f (edict *ent);

//
// g_items.c
//
void PrecacheItem (gitem *it);
void InitItems (void);
void SetItemNames (void);
gitem* FindItem (char *pickup_name);
gitem* FindItemByClassname (char *classname);
#define    ITEM_INDEX(x) ((x)-itemlist)
edict *Drop_Item (edict *ent, gitem *item);
void SetRespawn (edict *ent, float delay);
void ChangeWeapon (edict *ent);
void SpawnItem (edict *ent, gitem *item);
void Think_Weapon (edict *ent);
int ArmorIndex (edict *ent);
int PowerArmorType (edict *ent);
gitem* GetItemByIndex (int index);
bool Add_Ammo (edict *ent, gitem *item, int count);
void Touch_Item (edict *ent, edict *other, plane_t *plane, csurface_t *surf);

//
// g_utils.c
//
bool    KillBox (edict *ent);
void    G_ProjectSource (vec3_t point, vec3_t distance, vec3_t forward, vec3_t right, vec3_t result);
edict *G_Find (edict *from, int fieldofs, char *match);
edict *findradius (edict *from, vec3_t org, float rad);
edict *G_PickTarget (char *targetname);
void    G_UseTargets (edict *ent, edict *activator);
void    G_SetMovedir (vec3_t angles, vec3_t movedir);

void    G_InitEdict (edict *e);
edict* G_Spawn (void);
void    G_FreeEdict (edict *e);

void    G_TouchTriggers (edict *ent);
void    G_TouchSolids (edict *ent);

char* G_CopyString (char *in);

float* tv (float x, float y, float z);
char* vtos (vec3_t v);

float vectoyaw (vec3_t vec);
void vectoangles (vec3_t vec, vec3_t angles);

//
// g_combat.c
//
bool OnSameTeam (edict *ent1, edict *ent2);
bool CanDamage (edict *targ, edict *inflictor);
void T_Damage (edict *targ, edict *inflictor, edict *attacker, vec3_t dir, vec3_t point, vec3_t normal, int damage, int knockback, int dflags, int mod);
void T_RadiusDamage (edict *inflictor, edict *attacker, float damage, edict *ignore, float radius, int mod);

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
void monster_fire_bullet (edict *self, vec3_t start, vec3_t dir, int damage, int kick, int hspread, int vspread, int flashtype);
void monster_fire_shotgun (edict *self, vec3_t start, vec3_t aimdir, int damage, int kick, int hspread, int vspread, int count, int flashtype);
void monster_fire_blaster (edict *self, vec3_t start, vec3_t dir, int damage, int speed, int flashtype, int effect);
void monster_fire_grenade (edict *self, vec3_t start, vec3_t aimdir, int damage, int speed, int flashtype);
void monster_fire_rocket (edict *self, vec3_t start, vec3_t dir, int damage, int speed, int flashtype);
void monster_fire_railgun (edict *self, vec3_t start, vec3_t aimdir, int damage, int kick, int flashtype);
void monster_fire_bfg (edict *self, vec3_t start, vec3_t aimdir, int damage, int speed, int kick, float damage_radius, int flashtype);
void M_droptofloor (edict *ent);
void monster_think (edict *self);
void walkmonster_start (edict *self);
void swimmonster_start (edict *self);
void flymonster_start (edict *self);
void AttackFinished (edict *self, float time);
void monster_death_use (edict *self);
void M_CatagorizePosition (edict *ent);
bool M_CheckAttack (edict *self);
void M_FlyCheck (edict *self);
void M_CheckGround (edict *ent);

//
// g_misc.c
//
void ThrowHead (edict *self, char *gibname, int damage, int type);
void ThrowClientHead (edict *self, int damage);
void ThrowGib (edict *self, char *gibname, int damage, int type);
void BecomeExplosion1(edict *self);

//
// g_ai.c
//
void AI_SetSightClient (void);

void ai_stand (edict *self, float dist);
void ai_move (edict *self, float dist);
void ai_walk (edict *self, float dist);
void ai_turn (edict *self, float dist);
void ai_run (edict *self, float dist);
void ai_charge (edict *self, float dist);
int range (edict *self, edict *other);

void FoundTarget (edict *self);
bool infront (edict *self, edict *other);
bool visible (edict *self, edict *other);
bool FacingIdeal(edict *self);

//
// g_weapon.c
//
void ThrowDebris (edict *self, char *modelname, float speed, vec3_t origin);
bool fire_hit (edict *self, vec3_t aim, int damage, int kick);
void fire_bullet (edict *self, vec3_t start, vec3_t aimdir, int damage, int kick, int hspread, int vspread, int mod);
void fire_shotgun (edict *self, vec3_t start, vec3_t aimdir, int damage, int kick, int hspread, int vspread, int count, int mod);
void fire_blaster (edict *self, vec3_t start, vec3_t aimdir, int damage, int speed, int effect, bool hyper_in);
void fire_grenade (edict *self, vec3_t start, vec3_t aimdir, int damage, int speed, float timer, float damage_radius);
void fire_grenade2 (edict *self, vec3_t start, vec3_t aimdir, int damage, int speed, float timer, float damage_radius, bool held);
void fire_rocket (edict *self, vec3_t start, vec3_t dir, int damage, int speed, float damage_radius, int radius_damage);
void fire_rail (edict *self, vec3_t start, vec3_t aimdir, int damage, int kick);
void fire_bfg (edict *self, vec3_t start, vec3_t dir, int damage, int speed, float damage_radius);

//
// g_ptrail.c
//
void PlayerTrail_Init (void);
void PlayerTrail_Add (vec3_t spot);
void PlayerTrail_New (vec3_t spot);
edict *PlayerTrail_PickFirst (edict *self);
edict *PlayerTrail_PickNext (edict *self);
edict* PlayerTrail_LastSpot (void);

//
// g_client.c
//
void respawn (edict *ent);
void BeginIntermission (edict *targ);
void PutClientInServer (edict *ent);
void InitClientPersistant (gclient_t *client);
void InitClientResp (gclient_t *client);
void InitBodyQue (void);
void ClientBeginServerFrame (edict *ent);

//
// g_player.c
//
void player_pain (edict *self, edict *other, float kick, int damage);
void player_die (edict *self, edict *inflictor, edict *attacker, int damage, vec3_t point);

//
// g_svcmds.c
//
void    ServerCommand (void);
bool SV_FilterPacket (char *from);

//
// p_view.c
//
void ClientEndServerFrame (edict *ent);

//
// p_hud.c
//
void MoveClientToIntermission (edict *client);
void G_SetStats (edict *ent);
void G_SetSpectatorStats (edict *ent);
void G_CheckChaseStats (edict *ent);
void ValidateSelectedItem (edict *ent);
void DeathmatchScoreboardMessage (edict *client, edict *killer);

//
// g_pweapon.c
//
void PlayerNoise(edict *who, vec3_t where, int type);

//
// m_move.c
//
bool M_CheckBottom (edict *ent);
bool M_walkmove (edict *ent, float yaw, float dist);
void M_MoveToGoal (edict *ent, float dist);
void M_ChangeYaw (edict *ent);

//
// g_phys.c
//
void G_RunEntity (edict *ent);

//
// g_main.c
//
void SaveClientData (void);
void FetchClientEntData (edict *ent);

//
// g_chase.c
//
void UpdateChaseCam(edict *ent);
void ChaseNext(edict *ent);
void ChasePrev(edict *ent);
void GetChaseTarget(edict *ent);

//============================================================================

// client_t->anim_priority
#define    ANIM_BASIC        0        // stand / run
#define    ANIM_WAVE        1
#define    ANIM_JUMP        2
#define    ANIM_PAIN        3
#define    ANIM_ATTACK        4
#define    ANIM_DEATH        5
#define    ANIM_REVERSE    6

#endif // ____GAME_G_LOCAL_H__