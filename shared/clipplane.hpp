
#pragma once
#include "shared/defines.hpp"

// !!! if this is changed, it must be changed in asm_draw.h too !!!
struct clipplane_s
{
    vec3_t normal;
    float dist;
    clipplane_s* next = nullptr;
    byte leftedge;
    byte rightedge;
    byte reserved[2];
};