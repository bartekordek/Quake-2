#include "quake2/windows/window_util.h"
#include "quake2/windows/import_sdl.h"
#include <string>

SDL_Window *g_window{nullptr};

struct WindowsData
{
	int x{0};
	int y{0};
	int w{0};
	int h{0};
	qboolean fullscreen{e_false};
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

	g_window = SDL_CreateWindow ("Quake 2", x + 200, y + 200, w, h, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
	SDL_assert (g_window);
	SDL_GLContext context = SDL_GL_CreateContext (g_window);
	SDL_assert (context);

	g_windowdata.x			= x;
	g_windowdata.y			= y;
	g_windowdata.w			= w;
	g_windowdata.h			= h;
	g_windowdata.fullscreen = fullscreen;

	// SDL_SetWindowGrab (g_window, SDL_TRUE);

	return e_true;
}

void update_buffer()
{
	SDL_GL_SwapWindow (g_window);
}

void Get_window_attributes (int *inOut_x, int *inOut_y, int *inOut_w, int *inOut_h, qboolean *inOut_fullscreen)
{
	*inOut_x		  = g_windowdata.x;
	*inOut_y		  = g_windowdata.y;
	*inOut_w		  = g_windowdata.w;
	*inOut_h		  = g_windowdata.h;
	*inOut_fullscreen = g_windowdata.fullscreen;
}
