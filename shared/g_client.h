#ifndef ____SHARED_G_CLIENT_H__
#define ____SHARED_G_CLIENT_H__

#include "shared/defines.h"
#include "shared/player_state.h"
#include "shared/client_persistant.h"
#include "shared/client_respawn.h"

// this structure is cleared on each PutClientInServer(),
// except for 'client->pers'
struct gclient_s
{
    // known to server
    player_state_t    ps;                // communicated by server to clients
    int                ping;

    // private to game
    client_persistant_t    pers;
    client_respawn_t    resp;
    pmove_state_t        old_pmove;    // for detecting out-of-pmove changes

    qboolean    showscores;            // set layout stat
//ZOID
    qboolean    inmenu;                // in menu
    struct pmenuhnd_t* menu;                // current menu
//ZOID
    qboolean    showinventory;        // set layout stat
    qboolean    showhelp;
    qboolean    showhelpicon;

    int            ammo_index;

    int            buttons;
    int            oldbuttons;
    int            latched_buttons;

    qboolean    weapon_thunk;

    gitem_t        *newweapon;

    // sum up damage over an entire frame, so
    // shotgun blasts give a single big kick
    int            damage_armor;        // damage absorbed by armor
    int            damage_parmor;        // damage absorbed by power armor
    int            damage_blood;        // damage taken out of health
    int            damage_knockback;    // impact damage
    vec3_t        damage_from;        // origin for vector calculation

    float        killer_yaw;            // when dead, look at killer

    weaponstate_t    weaponstate;
    vec3_t        kick_angles;    // weapon kicks
    vec3_t        kick_origin;
    float        v_dmg_roll, v_dmg_pitch, v_dmg_time;    // damage kicks
    float        fall_time, fall_value;        // for view drop on fall
    float        damage_alpha;
    float        bonus_alpha;
    vec3_t        damage_blend;
    vec3_t        v_angle;            // aiming direction
    float        bobtime;            // so off-ground doesn't change it
    vec3_t        oldviewangles;
    vec3_t        oldvelocity;

    float        next_drown_time;
    int            old_waterlevel;
    int            breather_sound;

    int            machinegun_shots;    // for weapon raising

    // animation vars
    int            anim_end;
    int            anim_priority;
    qboolean    anim_duck;
    qboolean    anim_run;

    // powerup timers
    float        quad_framenum;
    float        invincible_framenum;
    float        breather_framenum;
    float        enviro_framenum;

    qboolean    grenade_blew_up;
    float        grenade_time;
    int            silencer_shots;
    int weapon_sound;

    float pickup_msg_time;

    float flood_locktill;        // locked from talking
    float flood_when[10];        // when messages were said
    int flood_whenhead;        // head pointer for when said

    float respawn_time;        // can respawn when time > this

    struct edict_s* chase_target;        // player we are chasing
    qboolean    update_chase;        // need to update chase info?

//ZOID
    void* ctf_grapple;        // entity of grapple
    int ctf_grapplestate;        // true if pulling
    float ctf_grapplereleasetime;    // time of grapple release
    float ctf_regentime;        // regen tech
    float ctf_techsndtime;
    float ctf_lasttechmsg;
    float menutime;            // time to update menu
    qboolean menudirty;
//ZOID
};

#endif // ____SHARED_G_CLIENT_H__