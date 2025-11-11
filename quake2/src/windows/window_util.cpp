#include "quake2/windows/window_util.h"
#include "quake2/windows/import_sdl.h"
#include "quake2/input/event_handler.h"
#include "quake2/input/input.h"
#include "quake2/input/in_win.h"
#include "quake2/client/keys.h"
#include "quake2/video/vid_dll.h"
#include "shared/logger.h"
#include <string>
#include <array>

std::int32_t SDL_Key_to_Quake_Key (std::int32_t inKey);	

SDL_Window *g_window{nullptr};
qboolean	g_foreGround{e_true};

struct WindowsData
{
	int win_x{0};
	int win_y{0};
	int win_w{0};
	int win_h{0};
	int screen_w{0};
	int screen_h{0};

	qboolean fullscreen{e_false};

	WinRect rect;
};

WindowsData g_windowdata;

void APIENTRY glDebugOutput (GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length,
							 const char *  // message
							 ,
							 const void *  // userParam
);


void APIENTRY glDebugOutput (GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char *message,
							 const void *userParam)
{
	if (id == 131185)
	{
		// Buffer detailed info: Buffer object [x] (bound to GL_ARRAY_BUFFER_ARB, usage hint is GL_STATIC_DRAW) will use VIDEO memory as the
		// source for buffer object operations.
		// https://stackoverflow.com/questions/62248552/opengl-debug-context-warning-will-use-video-memory-as-the-source-for-buffer-o
		// can be safely ignored.
		return;
	}

	std::string messageString = "glDebugOutput Severity: ";

	switch (severity)
	{
		case GL_DEBUG_SEVERITY_HIGH:
			messageString += "HIGH";
			break;
		case GL_DEBUG_SEVERITY_MEDIUM:
			messageString += "MEDIUM";
			break;
		case GL_DEBUG_SEVERITY_LOW:
			messageString += "LOW";
			break;
		case GL_DEBUG_SEVERITY_NOTIFICATION:
			messageString += "NOTIFICATION";
			break;
	}

	messageString += ", Source: ";
	switch (source)
	{
		case GL_DEBUG_SOURCE_API:
			messageString += "API";
			break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
			messageString += "WINDOW SYSTEM";
			break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER:
			messageString += "SHADER COMPILER";
			break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:
			messageString += "THIRD PARTY";
			break;
		case GL_DEBUG_SOURCE_APPLICATION:
			messageString += "APPLICATION";
			break;
		case GL_DEBUG_SOURCE_OTHER:
			messageString += "OTHER";
			break;
	}

	messageString += ", Type: ";
	switch (type)
	{
		case GL_DEBUG_TYPE_ERROR:
			messageString += "ERROR";
			break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
			messageString += "DEPRECATED BEHAVIOR";
			break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
			messageString += "UNDEFINED BEHAVIOR";
			break;
		case GL_DEBUG_TYPE_PORTABILITY:
			messageString += "PORTABILITY";
			break;
		case GL_DEBUG_TYPE_PERFORMANCE:
			messageString += "PERFORMANCE";
			break;
		case GL_DEBUG_TYPE_MARKER:
			messageString += "MARKER";
			break;
		case GL_DEBUG_TYPE_PUSH_GROUP:
			messageString += "PUSH GROUP";
			break;
		case GL_DEBUG_TYPE_POP_GROUP:
			messageString += "POP GROUP";
			break;
		case GL_DEBUG_TYPE_OTHER:
			messageString += "OTHER";
			break;
		default:
			messageString += "UNKOWN";
	}
	messageString += ", Message: ";
	messageString += message;

	return;
}

qboolean create_window (int x, int y, int w, int h, qboolean fullscreen)
{
	Uint32 flags = SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_OPENGL;

	if (fullscreen == e_true)
	{
		flags |= SDL_WINDOW_FULLSCREEN;
	}
	else
	{
		flags |= SDL_WINDOW_MOUSE_GRABBED;
		flags |= SDL_WINDOW_MOUSE_FOCUS;
		flags |= SDL_WINDOW_MOUSE_CAPTURE;
	}

	SDL_Init (SDL_INIT_VIDEO | SDL_INIT_EVENTS);

	SDL_GL_SetAttribute (SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute (SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute (SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute (SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
	SDL_GL_SetAttribute (SDL_GL_DOUBLEBUFFER, 1);

	g_window = SDL_CreateWindow ("Quake 2", x, y, w, h, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
	SDL_assert (g_window);
	SDL_GLContext context = SDL_GL_CreateContext (g_window);
	SDL_assert (context);

	g_windowdata.win_x		= x;
	g_windowdata.win_y		= y;
	g_windowdata.win_w		= w;
	g_windowdata.win_h		= h;
	g_windowdata.fullscreen = fullscreen;

	const int displayIndex	= SDL_GetWindowDisplayIndex (g_window);

	SDL_DisplayMode dm;
	SDL_GetCurrentDisplayMode (displayIndex, &dm);
	g_windowdata.screen_w = dm.w;
	g_windowdata.screen_h	 = dm.h;

	g_windowdata.rect.left	 = x;
	g_windowdata.rect.bottom = y + h;
	g_windowdata.rect.right	 = x + h;
	g_windowdata.rect.top	 = y;

	return e_true;
}

void update_buffer()
{
	SDL_GL_SwapWindow (g_window);
}

void Get_window_attributes (int *inOut_x, int *inOut_y, int *inOut_w, int *inOut_h, qboolean *inOut_fullscreen)
{
	if (inOut_x)
	{
		*inOut_x = g_windowdata.win_x;
	}

	if (inOut_y)
	{
		*inOut_y = g_windowdata.win_y;
	}

	if (inOut_w)
	{
		*inOut_w = g_windowdata.win_w;
	}

	if (inOut_h)
	{
		*inOut_h = g_windowdata.win_h;
	}

	if (inOut_fullscreen)
	{
		*inOut_fullscreen = g_windowdata.fullscreen;
	}
}

void Get_window_rect (WinRect *inOutRect)
{
	*inOutRect = g_windowdata.rect;
}

void Get_screen_attributes (int *inOut_w, int *inOut_h)
{
	*inOut_w = g_windowdata.screen_w;
	*inOut_h = g_windowdata.screen_h;
}

void Q2_SetMousePos (int x, int y)
{
	SDL_WarpMouseInWindow (g_window, x, y);
}

qboolean Get_Is_InForeground ()
{
	return g_foreGround;
}

std::array<std::int32_t, 128> sdlKeyToQuakeKey = {0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14,
												  15, 16, 17, 18, 19, 20, 21, 22, 13, 14, 15, 16, 17, 18, 19};

namespace Quake2
{
std::int32_t mouse_x{0};
std::int32_t mouse_y{0};
}  // namespace Quake2

void HandleWindowEvent (SDL_WindowEvent &event);
void HandleMousEvent (SDL_MouseMotionEvent &inEvent);
void HandleMouseClick (SDL_MouseButtonEvent &inEvent, std::uint64_t in_sys_msg_time);

void fetch_events ()
{
	// SDL_PEEK
	static SDL_Event event;
	std::uint64_t	 sys_msg_time = SDL_GetTicks64 ();
	while (SDL_PollEvent (&event) > 0)
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
			case SDL_MOUSEBUTTONDOWN:
			{
				HandleMouseClick (event.button, sys_msg_time);
				break;
			}
			case SDL_MOUSEBUTTONUP:
			{
				HandleMouseClick (event.button, sys_msg_time);
				break;
			}
			default:
				break;
		}
	}
}

std::int32_t SDL_Key_to_Quake_Key (std::int32_t inKey)
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
			SDL_ShowCursor (SDL_TRUE);
			
			break;
		}
		case SDL_WINDOWEVENT_LEAVE:
		{
			log_str ("SDL_WINDOWEVENT_LEAVE");
			SDL_ShowCursor (SDL_FALSE);
			
			break;
		}
		case SDL_WINDOWEVENT_FOCUS_GAINED:
		{
			log_str ("SDL_WINDOWEVENT_FOCUS_GAINED");
			g_foreGround = e_true;
			SDL_ShowCursor (SDL_FALSE);
			break;
		}
		case SDL_WINDOWEVENT_FOCUS_LOST:
		{
			log_str ("SDL_WINDOWEVENT_FOCUS_LOST");
			g_foreGround = e_false;
			SDL_ShowCursor (SDL_TRUE);
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

void HandleMouseClick (SDL_MouseButtonEvent &inEvent, std::uint64_t in_sys_msg_time)
{
	const qboolean state = (inEvent.state == SDL_PRESSED) ? (e_true) : (e_false);
	Key_Event (K_MOUSE1 + inEvent.button - 1, state, in_sys_msg_time);
}

void HandleMousEvent (SDL_MouseMotionEvent &inEvent)
{
	Quake2::mouse_x = inEvent.x;
	Quake2::mouse_y = inEvent.y;
}

void Get_mouse_pos (int *x, int *y)
{
	*x = Quake2::mouse_x + g_windowdata.rect.left;
	*y = Quake2::mouse_y + g_windowdata.rect.top;
}
