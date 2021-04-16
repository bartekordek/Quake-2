#ifndef ____SHARED_EDICT_H__
#define ____SHARED_EDICT_H__

#include "shared/entity_state.hpp"
#include "shared/moveinfo.hpp"
#include "shared/monsterinfo.hpp"
#include "shared/g_client.hpp"

// link_t is only used for entity area links now
typedef struct link_s
{
    struct link_s	*prev, *next;
} link_t;

// edict->solid values
typedef enum
{
SOLID_NOT,			// no interaction with other objects
SOLID_TRIGGER,		// only touch when inside, after moving
SOLID_BBOX,			// touch on edge
SOLID_BSP			// bsp clip, touch on edge
} solid_t;

struct edict;

struct edict
{
    entity_state s;
    gclient* client;// NULL if not a player
                             // the server expects the first part
                             // of gclient to be a player_state
                             // but the rest of it is opaque

    bool inuse;
    int linkcount;

    // FIXME: move these fields to a server private sv_entity_t
    link_t area;// linked to a division node or leaf

    int num_clusters;// if -1, use headnode instead
    int clusternums[MAX_ENT_CLUSTERS];
    int headnode;// unused if num_clusters != -1
    int areanum, areanum2;

    //================================
    int svflags;// SVF_NOCLIENT, SVF_DEADMONSTER, SVF_MONSTER, etc
    vec3_t mins, maxs;
    vec3_t absmin, absmax, size;
    solid_t solid;
    int clipmask;
    edict* owner;

    // DO NOT MODIFY ANYTHING ABOVE THIS, THE SERVER
    // EXPECTS THE FIELDS IN THAT ORDER!

    //================================
    int movetype;
    int flags;

    char    * model;
    float        freetime;            // sv.time when the object was freed

    //
    // only used locally in game, not by server
    //
    char    * message;
    char    * classname;
    int            spawnflags;

    float        timestamp;

    float        angle;            // set in qe3, -1 = up, -2 = down
    char    * target;
    char    * targetname;
    char    * killtarget;
    char    * team;
    char    * pathtarget;
    char    * deathtarget;
    char    * combattarget;
    edict* target_ent;

    float        speed, accel, decel;
    vec3_t        movedir;
    vec3_t        pos1, pos2;

    vec3_t        velocity;
    vec3_t        avelocity;
    int            mass;
    float        air_finished;
    float        gravity;        // per entity gravity multiplier (1.0 is normal)
                                // use for lowgrav artifact, flares

    edict    * goalentity;
    edict    * movetarget;
    float        yaw_speed;
    float        ideal_yaw;

    float        nextthink;
    void        (*prethink) (edict *ent);
    void        (*think)(edict *self);
    void        (*blocked)(edict *self, edict *other);    //move to moveinfo?
    void        (*touch)(edict *self, edict *other, plane_t *plane, csurface_t *surf);
    void        (*use)(edict *self, edict *other, edict *activator);
    void        (*pain)(edict *self, edict *other, float kick, int damage);
    void        (*die)(edict *self, edict *inflictor, edict *attacker, int damage, vec3_t point);

    float        touch_debounce_time;        // are all these legit?  do we need more/less of them?
    float        pain_debounce_time;
    float        damage_debounce_time;
    float        fly_sound_debounce_time;    //move to clientinfo
    float        last_move_time;

    int            health;
    int            max_health;
    int            gib_health;
    int            deadflag;
    bool    show_hostile;

    float        powerarmor_time;

    char    * map;            // target_changelevel

    int            viewheight;        // height above origin where eyesight is determined
    int            takedamage;
    int            dmg;
    int            radius_dmg;
    float        dmg_radius;
    int            sounds;            //make this a spawntemp var?
    int            count;

    edict    * chain;
    edict    * enemy;
    edict    * oldenemy;
    edict    * activator;
    edict    * groundentity;
    int            groundentity_linkcount;
    edict    * teamchain;
    edict    * teammaster;

    edict    * mynoise;        // can go in client only
    edict    * mynoise2;

    int            noise_index;
    int            noise_index2;
    float        volume;
    float        attenuation;

    // timing variables
    float        wait;
    float        delay;            // before firing targets
    float        random;

    float        teleport_time;

    int            watertype;
    int            waterlevel;

    vec3_t        move_origin;
    vec3_t        move_angles;

    // move this to clientinfo?
    int            light_level;

    int            style;            // also used as areaportal number

    struct gitem* item;// for bonus items

    // common data blocks
    moveinfo moveinfo;
    monsterinfo monsterinfo;
};

#endif // ____SHARED_EDICT_H__