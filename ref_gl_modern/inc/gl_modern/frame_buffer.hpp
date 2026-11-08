#pragma once

#include "shared/noncopyable.hpp"
#include <cstdint>

namespace Q2
{
class Shader;
class Framebuffer
{
public:
	Framebuffer ();
	~Framebuffer ();

	void create (std::int32_t width, std::int32_t height);
	void destroy ();
	void bind () const;
	void unbind () const;  // bind default FB (window)
	void render ();

	std::uint32_t color_texture () const
	{
		return m_color;
	}
	std::int32_t width () const
	{
		return m_width;
	}
	std::int32_t height () const
	{
		return m_height;
	}
	bool valid () const
	{
		return m_fbo != 0;
	}

	Q2_NONCOPYABLE (Framebuffer)

private:
	Shader		 *m_shader{nullptr};
	std::uint32_t m_vao{0u};
	std::uint32_t m_vbo{0u};

	std::uint32_t m_fbo{0};
	std::uint32_t m_color{0};
	std::uint32_t m_depth{0};  // renderbuffer for now
	std::int32_t  m_width{0};
	std::int32_t  m_height{0};
};
};	// namespace Q2
