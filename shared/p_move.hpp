
#pragma once
#include "shared/edict.hpp"
#include "shared/pmove_state.hpp"
#include "shared/trace.hpp"
#include "shared/usercmd.hpp"

struct pmove_t
{
    // state (in / out)
    pmove_state s;

    // command (in)
    usercmd_s cmd;
    bool snapinitial;  // if s has been changed outside pmove

    // results (out)
    int numtouch;
    edict* touchents[MAXTOUCH];

    vec3_t viewangles;  // clamped
    float viewheight;

    vec3_t mins, maxs;  // bounding box size

    edict* groundentity;
    int watertype;
    int waterlevel;

    // callbacks to test the world
    trace_t ( *trace )( vec3_t start, vec3_t mins, vec3_t maxs, vec3_t end );

    // trace_t trace( vec3_t start, vec3_t mins, vec3_t maxs, vec3_t end );

    int ( *pointcontents )( vec3_t point );
};