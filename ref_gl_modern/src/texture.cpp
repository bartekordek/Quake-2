#include "ref_gl/texture.hpp"
#include "ref_gl/shader.hpp"
//#ifdef _WIN32
//#include <windows.h>
//#endif
//#include <gl/GLU.h> // for some reason this one needs windows. :(
#include <GL/glew.h>
#include <vector>

namespace Q2
{
constexpr float g_float_epsilon = 0.000001f;
bool float_equal (float a, float b)
{
	return std::abs (a - b) < g_float_epsilon;
}


Texture::Texture ()
{
	const float m_firstVerticeX{0.f};
	const float m_firstVerticeZ{0.f};
	const std::array<float, 32> vertices  = createBufferData (m_scale);
	unsigned int indices[] = {
		0, 1, 2,  // first triangle
		1, 3, 0	  // second triangle
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
	attributeInfo.push_back (AttributeMeta ("tex", 2, 2, (int) GL_FLOAT, false, 8 * sizeof (float), (void *) (6 * sizeof (float))));

	for (const auto &att : attributeInfo)
	{
		glVertexAttribPointer (att.index, att.size, att.type, att.normalized, att.stride, att.pointer);
		glEnableVertexAttribArray (att.index);
	}

	m_shader = std::make_unique<Shader> ();
	m_shader->init ("../shaders/basic_texture.vert", "../shaders/basic_texture.frag");
	//m_shader->set_uniform_1i ("texture1", 0);
	//glUniform1i( glGetUniformLocation( m_shader->ID, "texture1" ), 0 );
}


void Texture::changeScale (float in_scale)
{
	if (float_equal (m_scale, in_scale))
	{
		return;
	}

	m_scale = in_scale;

	glBindBuffer (GL_ARRAY_BUFFER, m_vbo);

	const std::array<float, 32> vertices = createBufferData (m_scale);
	glBufferSubData (GL_ARRAY_BUFFER, 0, sizeof (float) * vertices.size(), vertices.data ());
}

std::array<float, 32> Texture::createBufferData (float in_scale)
{
	std::array<float, 32> result = {
		// positions               // colors                // texture coords
		1.0f,  1.0f,  0.0f, 1.0f, 0.0f, 0.0f, in_scale,     0.0f,  // top right
	   -1.0f, -1.0f,  0.0f, 0.0f, 1.0f, 0.0f,     0.0f, in_scale,  // bottom right
	    1.0f, -1.0f,  0.0f, 0.0f, 0.0f, 1.0f, in_scale, in_scale,  // bottom left
	   -1.0f,  1.0f,  0.0f, 1.0f, 1.0f, 0.0f,     0.0f,     0.0f   // top let
	};
	return result;
}

void Texture::init ()
{
}

void Texture::draw (const RenderData &inData)
{
	if (inData.alphaTest)
	{
		glDisable (GL_ALPHA_TEST);
	}

	glActiveTexture (GL_TEXTURE0);
	glBindTexture (GL_TEXTURE_2D, inData.id);

	glTexImage2D (GL_TEXTURE_2D, 0, inData.internal_format, inData.imgW, inData.imgH, 0, inData.format, inData.data_type, inData.data);
	glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	m_shader->use ();
	glBindVertexArray (m_vao);
	glDrawElements (GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	if (inData.alphaTest)
	{
		glEnable (GL_ALPHA_TEST);
	}
}

Texture::~Texture ()
{
}

}  // namespace Q2