#include "quake2/input/event_handler.h"
#include "quake2/client/keys.h"
#include "quake2/video/vid_dll.h"
#include "quake2/windows/import_sdl.h"
#include <array>
#include <cstdint>

std::array<std::int32_t, 128> sdlKeyToQuakeKey = {0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14,
												  15, 16, 17, 18, 19, 20, 21, 22, 13, 14, 15, 16, 17, 18, 19};

namespace Quake2
{
std::int32_t mouse_x{0};
std::int32_t mouse_y{0};
}


void log_str (const char *msg...)
{
	va_list args;
	va_start (args, msg);
	constexpr std::size_t bufferSize{1024};
	char				  buffer[bufferSize];
	snprintf (buffer, bufferSize, msg, args);
	va_end (args);

	char				  bufferOut[bufferSize];
	snprintf (bufferOut, bufferSize, "%s\n", buffer);

	OutputDebugString (bufferOut);
}


std::int32_t SDL_Key_to_Quake_Key(std::int32_t inKey)
{
	switch (inKey)
	{
		case SDL_SCANCODE_RETURN:
			return K_ENTER;
		case SDL_SCANCODE_ESCAPE:
			return K_ESCAPE;
		case SDL_SCANCODE_BACKSPACE:
			return K_BACKSPACE;
		case SDL_SCANCODE_TAB:
			return K_TAB;
		case SDL_SCANCODE_SPACE:
			return K_SPACE;
		case SDL_SCANCODE_UP:
			return K_UPARROW;
		case SDL_SCANCODE_DOWN:
			return K_DOWNARROW;
		case SDL_SCANCODE_LEFT:
			return K_LEFTARROW;
		case SDL_SCANCODE_RIGHT:
			return K_LEFTARROW;
		case SDL_SCANCODE_GRAVE:
			return 96;
		case SDL_SCANCODE_0:
			return 48;
		default:
			if ((inKey >= SDL_SCANCODE_1) && (SDL_SCANCODE_0))
			{
				return inKey + 19;
			}
			return inKey + 93;
	}
}

void HandleWindowEvent (SDL_WindowEvent &event);
void HandleMousEvent (SDL_MouseMotionEvent &inEvent);

void fetch_events ()
{
	// SDL_PEEK
	static SDL_Event event;
	std::uint64_t	 sys_msg_time = SDL_GetTicks64 ();
	if (SDL_PollEvent (&event) > 0)
	{
		switch (event.type)
		{
			case SDL_WINDOWEVENT:
			{
				HandleWindowEvent (event.window);
				break;
			}
			case SDL_SYSWMEVENT:
			case SDL_DISPLAYEVENT:
			{
				int i = 0;
				break;
			}
			case SDL_APP_WILLENTERFOREGROUND:
			{
				AppActivate (e_true, e_false);
				break;
			}
			case SDL_APP_WILLENTERBACKGROUND:
			case SDL_APP_DIDENTERFOREGROUND:
			{
				AppActivate (e_false, e_false);
				break;
			}
			case SDL_KEYDOWN:
			{
				Key_Event (SDL_Key_to_Quake_Key (event.key.keysym.scancode), e_true, sys_msg_time);
				break;
			}
			case SDL_KEYUP:
			{
				Key_Event (SDL_Key_to_Quake_Key (event.key.keysym.scancode), e_false, sys_msg_time);
				break;
			}
			case SDL_MOUSEMOTION:
			{
				HandleMousEvent (event.motion);
				break;
			}
			default:
				break;
		}		
	}
}

void HandleWindowEvent (SDL_WindowEvent &event)
{
	switch (event.event)
	{
		case SDL_WINDOWEVENT_SHOWN:
		{
			log_str ("SDL_WINDOWEVENT_SHOWN");
			AppActivate (e_true, e_false);
			break;
		}
		case SDL_WINDOWEVENT_HIDDEN:
		{
			log_str ("SDL_WINDOWEVENT_HIDDEN");
			AppActivate (e_false, e_false);
			break;
		}
		case SDL_WINDOWEVENT_EXPOSED:
		{
			log_str ("SDL_WINDOWEVENT_EXPOSED");
			break;
		}
		case SDL_WINDOWEVENT_MOVED:
		{
			log_str ("SDL_WINDOWEVENT_MOVED");
			break;
		}
		case SDL_WINDOWEVENT_MINIMIZED:
		{
			log_str ("SDL_WINDOWEVENT_MINIMIZED");
			break;
		}
		case SDL_WINDOWEVENT_ENTER:
		{
			log_str ("SDL_WINDOWEVENT_ENTER");
			break;
		}
		case SDL_WINDOWEVENT_LEAVE:
		{
			log_str ("SDL_WINDOWEVENT_LEAVE");
			break;
		}
		case SDL_WINDOWEVENT_FOCUS_GAINED:
		{
			log_str ("SDL_WINDOWEVENT_FOCUS_GAINED");
			SDL_ShowCursor (SDL_FALSE);
//			SDL_CaptureMouse (SDL_TRUE);
			//SDL_SetRelativeMouseMode (SDL_TRUE);
			break;
		}
		case SDL_WINDOWEVENT_FOCUS_LOST:
		{
			log_str ("SDL_WINDOWEVENT_FOCUS_LOST");
			SDL_ShowCursor (SDL_TRUE);
			//SDL_CaptureMouse (SDL_FALSE);
			//SDL_SetRelativeMouseMode (SDL_FALSE);
			break;
		}
		case SDL_WINDOWEVENT_TAKE_FOCUS:
		{
			log_str ("SDL_WINDOWEVENT_TAKE_FOCUS");
			break;
		}
		default:
			break;
	}
}

void HandleMousEvent (SDL_MouseMotionEvent &inEvent)
{
	Quake2::mouse_x = inEvent.x;
	Quake2::mouse_y = inEvent.y;
}

void Get_mouse_pos (int *x, int *y)
{
	*x = Quake2::mouse_x;
	*y = Quake2::mouse_y;
}
