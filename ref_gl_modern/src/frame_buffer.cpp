#include "gl_modern/frame_buffer.hpp"
#include "gl_modern/shader.hpp"
#include "shared/assert.h"
#include <GL/glew.h>
#include <glm/vec2.hpp>
#include <vector>

namespace Q2
{

Framebuffer::Framebuffer ()
{
}

void Framebuffer::create (std::int32_t width, std::int32_t height)
{
	destroy ();	 // safe if called again on resize

	m_width	 = width;
	m_height = height;

	glGenFramebuffers (1, &m_fbo);
	glBindFramebuffer (GL_FRAMEBUFFER, m_fbo);

	// Color attachment (empty texture — no CPU pixels)
	glGenTextures (1, &m_color);
	glBindTexture (GL_TEXTURE_2D, m_color);
	glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D (GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_color, 0);

	// Depth as renderbuffer (we don't sample it yet)
	glGenRenderbuffers (1, &m_depth);
	glBindRenderbuffer (GL_RENDERBUFFER, m_depth);
	glRenderbufferStorage (GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
	glFramebufferRenderbuffer (GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depth);

	GLenum status = glCheckFramebufferStatus (GL_FRAMEBUFFER);
	Q2_Assert (status == GL_FRAMEBUFFER_COMPLETE, "FBO incomplete");

	glBindFramebuffer (GL_FRAMEBUFFER, 0);

	m_shader = ShaderStore::get_instance ().get_or_create_shader ({"shaders/fbo.vert", "shaders/fbo.frag"});

	const glm::vec2	   tr{1.f, 1.f};
	std::vector<float> quadVertices = {-tr.x, tr.y, 0.0f, 1.0f, -tr.x, -tr.y, 0.0f, 0.0f, tr.x, -tr.y, 1.0f, 0.0f,
									   tr.x,  tr.y, 1.0f, 1.0f, -tr.x, tr.y,  0.0f, 1.0f, tr.x, -tr.y, 1.0f, 0.0f};
	const std::size_t  dataSize		= quadVertices.size () * sizeof (float);  // should be 96 in Win64.

	glGenVertexArrays (1, &m_vao);
	glGenBuffers (1, &m_vbo);

	glBindVertexArray (m_vao);
	glBindBuffer (GL_ARRAY_BUFFER, m_vbo);
	glBufferData (GL_ARRAY_BUFFER, dataSize, quadVertices.data (), GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer (0, 2, GL_FLOAT, GL_FALSE, 16, nullptr);
	glEnableVertexAttribArray (0);

	// TexCoord attribute
	glVertexAttribPointer (1, 2, GL_FLOAT, GL_FALSE, 16, reinterpret_cast<void *> (2 * sizeof (float)));
	glEnableVertexAttribArray (1);

	glBindVertexArray (0);
	glBindBuffer (GL_ARRAY_BUFFER, 0);
}

void Framebuffer::render ()
{
	if (!m_shader || m_vao == 0u || m_color == 0u)
	{
		return;
	}

	// Caller should already have unbound the FBO (see GLimp_EndFrame).
	glViewport (0, 0, m_width, m_height);
	glDisable (GL_DEPTH_TEST);

	m_shader->use ();

	glActiveTexture (GL_TEXTURE0);
	glBindTexture (GL_TEXTURE_2D, m_color);
	m_shader->set_uniform_1i ("screenTexture", 0);

	glBindVertexArray (m_vao);
	glDrawArrays (GL_TRIANGLES, 0, 6);
	glBindVertexArray (0);
}

void Framebuffer::bind () const
{
	glBindFramebuffer (GL_FRAMEBUFFER, m_fbo);
	glViewport (0, 0, m_width, m_height);
}

void Framebuffer::unbind () const
{
	glBindFramebuffer (GL_FRAMEBUFFER, 0);
}

void Framebuffer::destroy ()
{
	if (m_vao)
	{
		glDeleteVertexArrays (1, &m_vao);
		m_vao = 0;
	}
	if (m_vbo)
	{
		glDeleteBuffers (1, &m_vbo);
		m_vbo = 0;
	}
	if (m_depth)
	{
		glDeleteRenderbuffers (1, &m_depth);
		m_depth = 0;
	}
	if (m_color)
	{
		glDeleteTextures (1, &m_color);
		m_color = 0;
	}
	if (m_fbo)
	{
		glDeleteFramebuffers (1, &m_fbo);
		m_fbo = 0;
	}
	m_width = m_height = 0;
}

Framebuffer::~Framebuffer ()
{
}

}  // namespace Q2
