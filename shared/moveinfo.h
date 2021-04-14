#ifndef ____SHARED_MOVEINFO_H__
#define ____SHARED_MOVEINFO_H__

#include "shared/defines.h"

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

#endif // ____SHARED_MOVEINFO_H__