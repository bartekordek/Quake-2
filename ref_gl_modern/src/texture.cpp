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
Texture::Texture ()
{
	const float m_firstVerticeX{0.f};
	const float m_firstVerticeZ{0.f};
	float vertices[] = {
		// positions               // colors                // texture coords
		0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f,  1.0f, 0.0f,  // top right
	   -0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 0.0f,  0.0f, 1.0f,  // bottom right
	    0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f,  1.0f, 1.0f,  // bottom left
	   -0.5f,  0.5f,  0.0f, 1.0f, 1.0f, 0.0f,  0.0f, 0.0f   // top let
	};
	unsigned int indices[] = {
		0, 1, 2,  // first triangle
		1, 3, 0	  // second triangle
	};


	glGenVertexArrays (1, &m_vao);
	glGenBuffers (1, &m_vbo);
	glGenBuffers (1, &m_ebo);

	glBindVertexArray (m_vao);

	glBindBuffer (GL_ARRAY_BUFFER, m_vbo);
	glBufferData (GL_ARRAY_BUFFER, sizeof (vertices), vertices, GL_STATIC_DRAW);

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

void Texture::init ()
{
}

void Texture::draw (const RenderData &inData)
{
	glActiveTexture (GL_TEXTURE0);
	glBindTexture (GL_TEXTURE_2D, inData.id);

	glTexImage2D (GL_TEXTURE_2D, 0, inData.internal_format, inData.imgW, inData.imgH, 0, inData.format, inData.data_type, inData.data);
	glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//drawStatic (inData);
	m_shader->use ();
	glBindVertexArray (m_vao);
	glDrawElements (GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Texture::drawStatic (const RenderData &rd)
{
	if (rd.alphaTest)
	{
		glDisable (GL_ALPHA_TEST);
	}

	glBegin (GL_QUADS);
	glTexCoord2f (rd.s[0], rd.t[0]);
	glVertex2f (rd.x, rd.y);
	glTexCoord2f (rd.s[1], rd.t[1]);
	glVertex2f (rd.x + rd.w, rd.y);
	glTexCoord2f (rd.s[2], rd.t[2]);
	glVertex2f (rd.x + rd.w, rd.y + rd.h);
	glTexCoord2f (rd.s[3], rd.t[3]);
	glVertex2f (rd.x, rd.y + rd.h);
	glEnd ();

	if (rd.alphaTest)
	{
		glEnable (GL_ALPHA_TEST);
	}
}
Texture::~Texture ()
{
}
}  // namespace Q2