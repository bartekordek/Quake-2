#pragma once

#include "shared/defines.hpp"

// entity_state is the information conveyed from the server
// in an update message about entities that the client will
// need to render in some way
struct entity_state
{
    int        number;            // edict index

    vec3_t    origin;
    vec3_t    angles;
    vec3_t    old_origin;        // for lerping
    int        modelindex;
    int        modelindex2, modelindex3, modelindex4;    // weapons, CTF flags, etc
    int        frame;
    int        skinnum;
    unsigned int        effects;        // PGM - we're filling it, so it needs to be unsigned
    int        renderfx;
    int        solid;            // for client side prediction, 8*(bits 0-4) is x/y radius
                            // 8*(bits 5-9) is z down distance, 8(bits10-15) is z up
                            // gi.linkentity sets this properly
    int        sound;            // for looping sounds, to guarantee shutoff
    int        event;            // impulse events -- muzzle flashes, footsteps, etc
                            // events only go out for a single frame, they
                            // are automatically cleared each frame
};