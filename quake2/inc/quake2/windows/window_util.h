#ifndef __window_util_h__
#define __window_util_h__

#include "shared/config.h"
#include "shared/boolean.h"

EXTERNC qboolean create_window (int x, int y, int w, int h, qboolean fullscreen);
EXTERNC void	 update_buffer ();
EXTERNC void	 Get_window_attributes (int *inOut_x, int *inOut_y, int *inOut_w, int *inOut_h, qboolean *inOut_fullscreen);

#endif	// __window_util_h__