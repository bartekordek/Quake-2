
#pragma once
#include "shared/defines.hpp"

// usercmd_s is sent to the server each client frame
typedef struct usercmd_s
{
    byte msec;
    byte buttons;
    short angles[3];
    short forwardmove, sidemove, upmove;
    byte impulse;     // remove?
    byte lightlevel;  // light level the player is standing on
};
