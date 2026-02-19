#include "ref_gl/quad.hpp"
#include "ref_gl/attribute_meta.hpp"
#include "ref_gl/shader.hpp"
#include <GL/glew.h>
#include <vector>

namespace Q2
{
Quad::Quad ()
{
	init ();
}

Quad::Quad (const char *in_name)
{
	init ();
	m_name = in_name;
	set_name (in_name);
}

void Quad::init ()
{
	initialize_data ();
	const std::array<float, 28> vertices  = createBufferData (m_raw_data);
	unsigned int				indices[] = {
		   0, 1, 2,	 // first triangle
		   1, 3, 0	 // second triangle
	   };

	glGenVertexArrays (1, &m_vao);
	glGenBuffers (1, &m_vbo);
	glGenBuffers (1, &m_ebo);

	glBindVertexArray (m_vao);

	glBindBuffer (GL_ARRAY_BUFFER, m_vbo);
	glBufferData (GL_ARRAY_BUFFER, sizeof (vertices), vertices.data (), GL_STATIC_DRAW);

	glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData (GL_ELEMENT_ARRAY_BUFFER, sizeof (indices), indices, GL_STATIC_DRAW);

	std::vector<AttributeMeta> attributeInfo;
	attributeInfo.push_back (AttributeMeta ("pos", 0, 3, (int) GL_FLOAT, false, 8 * sizeof (float), nullptr));
	attributeInfo.push_back (AttributeMeta ("col", 1, 3, (int) GL_FLOAT, false, 8 * sizeof (float), (void *) (3 * sizeof (float))));

	for (const auto &att : attributeInfo)
	{
		glVertexAttribPointer (att.index, att.size, att.type, att.normalized, att.stride, att.pointer);
		glEnableVertexAttribArray (att.index);
	}

	m_shader = std::make_unique<Shader> ();
	m_shader->init ("../shaders/quad.vert", "../shaders/quad.frag");
}

void Quad::set_name (const char *in_name)
{
	m_name = in_name;
	char buffer[256u];

	sprintf (buffer, "VAO: %s", in_name);
	glObjectLabel (GL_VERTEX_ARRAY, m_vao, -1, buffer);

	sprintf (buffer, "VBO: %s", in_name);
	glObjectLabel (GL_BUFFER, m_vbo, -1, buffer);

	sprintf (buffer, "IBO: %s", in_name);
	glObjectLabel (GL_BUFFER, m_ebo, -1, buffer);

	m_shader->set_name (in_name);
}

void Quad::initialize_data()
{
	m_raw_data[0].color = m_data.color;
	m_raw_data[0].pos	= {m_data.pos.x + m_data.size.w, m_data.pos.y + m_data.size.h, 0.f};

	m_raw_data[1].color = m_data.color;
	m_raw_data[1].pos	= {m_data.pos.x, m_data.pos.y, 0.f};

	m_raw_data[2].color = m_data.color;
	m_raw_data[2].pos	= {m_data.pos.x + m_data.size.w, m_data.pos.y, 0.f};

	m_raw_data[3].color = m_data.color;
	m_raw_data[3].pos	= {m_data.pos.x, m_data.pos.y + m_data.size.h, 0.f};
}

std::array<float, 28> Quad::createBufferData (const std::array<QuadVertex, 4> &in_arg) const
{
	std::array<float, 28> result = {
		// positions               // colors            
		in_arg[0].pos.x, in_arg[0].pos.y, in_arg[0].pos.z, in_arg[0].color.r, in_arg[0].color.g, in_arg[0].color.b, in_arg[0].color.a,  // top right
		in_arg[1].pos.x, in_arg[1].pos.y, in_arg[1].pos.z, in_arg[1].color.r, in_arg[1].color.g, in_arg[1].color.b, in_arg[1].color.a,  // bottom right
		in_arg[2].pos.x, in_arg[2].pos.y, in_arg[2].pos.z, in_arg[2].color.r, in_arg[2].color.g, in_arg[2].color.b, in_arg[2].color.a,  // bottom left
		in_arg[3].pos.x, in_arg[3].pos.y, in_arg[3].pos.z, in_arg[3].color.r, in_arg[2].color.g, in_arg[3].color.b, in_arg[3].color.a  // top let
	};
	return result;
}

void Quad::draw (const DrawData &in_data)
{
	m_data = in_data;
	update_buffer_data ();
	m_shader->use ();
	glBindVertexArray (m_vao);
	glDrawElements (GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void Quad::update_buffer_data ()
{
	initialize_data ();

	const std::array<float, 28> vertices = createBufferData (m_raw_data);
	glBindBuffer (GL_ARRAY_BUFFER, m_vbo);
	glBufferSubData (GL_ARRAY_BUFFER, 0, sizeof (float) * vertices.size (), vertices.data ());
}

Quad::~Quad ()
{
}

}  // namespace Q2
