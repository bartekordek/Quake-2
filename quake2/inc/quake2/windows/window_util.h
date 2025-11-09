#ifndef __window_util_h__
#define __window_util_h__

#include "shared/config.h"
#include "shared/boolean.h"

EXTERNC qboolean create_window (int x, int y, int w, int h, qboolean fullscreen);
EXTERNC void	 update_buffer ();

#endif	// __window_util_h__