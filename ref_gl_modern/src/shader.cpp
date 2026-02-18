#include "ref_gl/shader.hpp"
#include "shared/assert.h"
#include <GL/glew.h>
#include <fstream>
#include <sstream>

namespace Q2
{
Shader::Shader ()
{
}

void Shader::init (const char *vertex_shader, const char *fragment_shader)
{
	m_program_id = glCreateProgram ();
	m_vertex_id	 = glCreateShader (GL_VERTEX_SHADER);
	m_fragment_id = glCreateShader (GL_FRAGMENT_SHADER);

	std::string	  vertex_code;
	std::string	  fragment_code;
	std::ifstream vertex_file;
	std::ifstream fragment_file;

	std::stringstream vertex_ss;
	std::stringstream fragment_ss;

	vertex_file.open (vertex_shader);
	fragment_file.open (fragment_shader);

	vertex_ss << vertex_file.rdbuf ();
	fragment_ss << fragment_file.rdbuf ();

	vertex_file.close ();
	fragment_file.close ();

	vertex_code	  = vertex_ss.str ();
	fragment_code = fragment_ss.str ();

	const char *frag_code_cstr = fragment_code.c_str ();
	glShaderSource (m_fragment_id, 1, &frag_code_cstr, NULL);
	glCompileShader (m_fragment_id);

	int	 success{1};
	char infoLog[512];
	glGetShaderiv (m_fragment_id, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog (m_fragment_id, 512, NULL, infoLog);
		Q2_Assert (false, infoLog);
	}

	const char *vert_code_cstr = vertex_code.c_str ();
	glShaderSource (m_vertex_id, 1, &vert_code_cstr, NULL);
	glCompileShader (m_vertex_id);
	if (!success)
	{
		glGetShaderInfoLog (m_vertex_id, 512, NULL, infoLog);
		Q2_Assert (false, infoLog);
	}

	glAttachShader (m_program_id, m_vertex_id);
	glAttachShader (m_program_id, m_fragment_id);
	glLinkProgram (m_program_id);
}

void Shader::use ()
{
	glUseProgram (m_program_id);
}

void Shader::set_uniform_1i (const std::string &name, std::int32_t value) const
{
	glUseProgram (m_program_id);

	const std::int32_t location = glGetUniformLocation (m_program_id, name.c_str ());
	glUniform1i (location, value);
}

Shader::~Shader ()
{
}
}  // namespace Q2
