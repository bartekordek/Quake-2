#ifndef ____SHARED_PLAYER_STATE_H__
#define ____SHARED_PLAYER_STATE_H__

// player_state_t is the information needed in addition to pmove_state_t
// to rendered a view.  There will only be 10 player_state_t sent each second,
// but the number of pmove_state_t changes will be reletive to client
// frame rates
typedef struct
{
    pmove_state_t    pmove;        // for prediction

    // these fields do not need to be communicated bit-precise

    vec3_t        viewangles;        // for fixed views
    vec3_t        viewoffset;        // add to pmovestate->origin
    vec3_t        kick_angles;    // add to view direction to get render angles
                                // set by weapon kicks, pain effects, etc

    vec3_t        gunangles;
    vec3_t        gunoffset;
    int            gunindex;
    int            gunframe;

    float        blend[4];        // rgba full screen effect

    float        fov;            // horizontal field of view

    int            rdflags;        // refdef flags

    short        stats[MAX_STATS];        // fast status bar updates
} player_state_t;

#endif // ____SHARED_PLAYER_STATE_H__