#ifndef __in_win_h__
#define __in_win_h__

#include "shared/config.h"
#include "shared/cvar.h"

typedef struct
{
	int x;
	int y;
} MousePos;

EXTERNC cvar_t *in_joystick;
EXTERNC void	IN_ActivateMouse ();
EXTERNC void	IN_DeactivateMouse ();

#endif	// __in_win_h__