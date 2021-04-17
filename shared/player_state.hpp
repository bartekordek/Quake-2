#ifndef ____SHARED_PLAYER_STATE_H__
#define ____SHARED_PLAYER_STATE_H__

#include "shared/pmove_state.hpp"

// player_state is the information needed in addition to pmove_state
// to rendered a view.  There will only be 10 player_state sent each second,
// but the number of pmove_state changes will be reletive to client
// frame rates
struct player_state
{
    pmove_state pmove;  // for prediction

    // these fields do not need to be communicated bit-precise

    vec3_t viewangles;   // for fixed views
    vec3_t viewoffset;   // add to pmovestate->origin
    vec3_t kick_angles;  // add to view direction to get render angles
                         // set by weapon kicks, pain effects, etc

    vec3_t gunangles;
    vec3_t gunoffset;
    int gunindex;
    int gunframe;

    float blend[4];  // rgba full screen effect

    float fov;  // horizontal field of view

    int rdflags;  // refdef flags

    short stats[MAX_STATS];  // fast status bar updates
};

#endif  // ____SHARED_PLAYER_STATE_H__