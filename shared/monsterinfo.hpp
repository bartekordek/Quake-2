#ifndef ____SHARED_MONSTERINFO_H__
#define ____SHARED_MONSTERINFO_H__

#include "shared/defines.hpp"
#include "shared/mmove.hpp"

struct monsterinfo
{
    mmove_t* currentmove;
    int            aiflags;
    int            nextframe;
    float        scale;

    void        (*stand)(edict *self);
    void        (*idle)(edict *self);
    void        (*search)(edict *self);
    void        (*walk)(edict *self);
    void        (*run)(edict *self);
    void        (*dodge)(edict *self, edict *other, float eta);
    void        (*attack)(edict *self);
    void        (*melee)(edict *self);
    void        (*sight)(edict *self, edict *other);
    bool    (*checkattack)(edict *self);

    float        pausetime;
    float        attack_finished;

    vec3_t        saved_goal;
    float        search_time;
    float        trail_time;
    vec3_t        last_sighting;
    int            attack_state;
    int            lefty;
    float        idle_time;
    int linkcount;

    int power_armor_type;
    int power_armor_power;
};


#endif // ____SHARED_MONSTERINFO_H__