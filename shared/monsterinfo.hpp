#ifndef ____SHARED_MONSTERINFO_H__
#define ____SHARED_MONSTERINFO_H__

#include "shared/defines.hpp"
#include "shared/mmove.hpp"

typedef struct
{
    mmove_t* currentmove;
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
    int linkcount;

    int power_armor_type;
    int power_armor_power;
} monsterinfo_t;


#endif // ____SHARED_MONSTERINFO_H__