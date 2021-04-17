#ifndef ____SHARED_PMOVE_STATE_H__
#define ____SHARED_PMOVE_STATE_H__

#include "shared/defines.hpp"

// this structure needs to be communicated bit-accurate
// from the server to the client to guarantee that
// prediction stays in sync, so no floats are used.
// if any part of the game code modifies this struct, it
// will result in a prediction error of some degree.
struct pmove_state
{
    pmtype_t pm_type;

    short origin[3];    // 12.3
    short velocity[3];  // 12.3
    byte pm_flags;      // ducked, jump_held, etc
    byte pm_time;       // each unit = 8 ms
    short gravity;
    short delta_angles[3];  // add to command angles to get view direction
                            // changed by spawns, rotating objects, and
                            // teleporters
};

#endif  // ____SHARED_PMOVE_STATE_H__