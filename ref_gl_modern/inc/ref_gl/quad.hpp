#pragma once

#include "math/vector.h"
#include "shared/noncopyable.hpp"
#include <array>
#include <memory>
#include <string>

namespace Q2
{
class Shader;

struct Position
{
	float x{0.f};
	float y{0.f};
	float z{0.f};
};

struct Size
{
	float w{1.f};
	float h{1.f};
};

struct Color
{
	float r{0.f};
	float g{0.f};
	float b{0.f};
	float a{1.f};
};

struct DrawData
{
	Position pos;
	Size	 size;
	Color	 color;
};

struct QuadVertex
{
	Position pos;
	Color	 color;
};

class Quad
{
public:
	Quad ();
	
	Quad (const char *in_name);

	void draw (const DrawData &in_data);
	void set_name (const char *in_name);

	~Quad ();

	Q2_NONCOPYABLE (Quad)
protected:
private:
	void					  init ();
	void					  initialize_data ();
	std::array<float, 28>	  createBufferData (const std::array<QuadVertex, 4> &in_arg) const;
	void					  update_buffer_data ();
	std::string				  m_name;
	DrawData				  m_data;
	std::array<QuadVertex, 4> m_raw_data;
	std::uint32_t			  m_vao{0u};
	std::uint32_t			  m_vbo{0u};
	std::uint32_t			  m_ebo{0u};
	std::unique_ptr<Shader>	  m_shader;
};
}  // namespace Q2
