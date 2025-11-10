#ifndef __usercmd_h__
#define __usercmd_h__

#include "shared/defines.h"

// usercmd_t is sent to the server each client frame
typedef struct usercmd_s
{
	byte  msec;
	byte  buttons;
	short angles[3];
	short forwardmove, sidemove, upmove;
	byte  impulse;	   // remove?
	byte  lightlevel;  // light level the player is standing on
} usercmd_t;

#endif	// __usercmd_h__