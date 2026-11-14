#include "quake2/renderer/gl_modern/renderer.hpp"
#include "quake2/renderer/gl_modern/frame_buffer.hpp"
#include "quake2/renderer/gl_modern/gl_local.h"
#include "shared/cvar.h"
#include "shared/ref.h"

#include <cstring>

extern Q2::Framebuffer g_framebuffer;
extern refimport_t	   ri;

namespace gl_modern
{
Renderer::Renderer ()
{
}

void Renderer::initialize ()
{
}

void Renderer::begin_frame ()
{
}

void Renderer::end_frame ()
{
	// Drain any pending GL errors so they cannot block the resolve/swap.
	while (glGetError () != GL_NO_ERROR)
	{
	}

	g_framebuffer.render ();

	if (stricmp (gl_drawbuffer->string, "GL_BACK") == 0)
	{
		ri.Swap_buffers ();
	}
}

Renderer::~Renderer ()
{
}
}  // namespace gl_modern