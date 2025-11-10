#ifndef __window_util_h__
#define __window_util_h__

#include "shared/config.h"
#include "shared/boolean.h"

typedef struct 
{
	int left;
	int top;
	int right;
	int bottom;
} WinRect;

EXTERNC qboolean create_window (int x, int y, int w, int h, qboolean fullscreen);
EXTERNC void	 update_buffer ();
EXTERNC void	 Get_window_attributes (int *inOut_x, int *inOut_y, int *inOut_w, int *inOut_h, qboolean *inOut_fullscreen);
EXTERNC void	 Get_screen_attributes (int *inOut_w, int *inOut_h);
EXTERNC void	 Get_window_rect (WinRect* inOutRect);

#endif	// __window_util_h__