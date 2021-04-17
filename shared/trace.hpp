
#pragma once
#include "shared/defines.hpp"
#include "shared/edict.hpp"
#include "shared/plane.hpp"
#include "shared/surface.hpp"

// a trace is returned when a box is swept through the world
struct trace_t
{
    bool allsolid;        // if true, plane is not valid
    bool startsolid;      // if true, the initial point was in a solid area
    float fraction;       // time completed, 1.0 = didn't hit anything
    vec3_t endpos;        // final position
    plane_s plane;        // surface normal at impact
    csurface_s* surface;  // surface hit
    int contents;         // contents on other side of surface hit
    edict* ent;           // not set by CM_*() functions
};