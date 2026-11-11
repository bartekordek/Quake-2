#include "quake2/renderer/gl_modern/shader.hpp"
#include "shared/assert.h"
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <cstring>
#include <fstream>
#include <functional>
#include <sstream>

namespace Q2
{

ShaderStore::ShaderStore ()
{
}

ShaderStore &ShaderStore::get_instance ()
{
	static ShaderStore s_instance;
	return s_instance;
}

Shader *ShaderStore::get_or_create_shader (const ShaderKey &key)
{
	Shader *result = nullptr;

	const auto it  = m_shaders.find (key);
	if (it != m_shaders.end ())
	{
		result = it->second.get ();
	}
	else
	{
		auto shader = std::make_unique<Shader> ();
		result		= shader.get ();
		shader->init (key.vertex.c_str (), key.fragment.c_str (), (key.vertex + "+" + key.fragment).c_str ());
		m_shaders.emplace (key, std::move (shader));
	}

	return result;
}

bool ShaderKey::operator== (const ShaderKey &other) const
{
	return vertex == other.vertex && fragment == other.fragment;
}

bool ShaderKey::operator< (const ShaderKey &other) const
{
	if (vertex != other.vertex)
		return vertex < other.vertex;
	return fragment < other.fragment;
}

std::size_t ShaderKeyHash::operator() (const ShaderKey &key) const noexcept
{
	const std::size_t h1 = std::hash<std::string>{}(key.vertex);
	const std::size_t h2 = std::hash<std::string>{}(key.fragment);
	return h1 ^ (h2 + 0x9e3779b97f4a7c15ULL + (h1 << 6) + (h1 >> 2));
}

ShaderStore::~ShaderStore ()
{
}

Shader::Shader ()
{
}

void Shader::init (const char *vertex_shader, const char *fragment_shader, const char *in_name)
{
	Q2_Assert (in_name != nullptr && strlen (in_name) > 0, "EMPTY NAME!");
	m_fragment_shader_path = fragment_shader;
	m_vertex_shader_path   = vertex_shader;

	m_program_id		   = glCreateProgram ();
	m_vertex_id			   = glCreateShader (GL_VERTEX_SHADER);
	m_fragment_id		   = glCreateShader (GL_FRAGMENT_SHADER);

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

	vertex_code				   = vertex_ss.str ();
	fragment_code			   = fragment_ss.str ();

	const char *frag_code_cstr = fragment_code.c_str ();
	glShaderSource (m_fragment_id, 1, &frag_code_cstr, NULL);
	glCompileShader (m_fragment_id);

	int	 success{1};
	char infoLog[512];
	glGetShaderiv (m_fragment_id, GL_COMPILE_STATUS, &success);
	if (success != GL_TRUE)
	{
		glGetShaderInfoLog (m_fragment_id, 512, NULL, infoLog);
		Q2_Assert (false, infoLog);
	}

	const char *vert_code_cstr = vertex_code.c_str ();
	glShaderSource (m_vertex_id, 1, &vert_code_cstr, NULL);
	glCompileShader (m_vertex_id);
	glGetShaderiv (m_vertex_id, GL_COMPILE_STATUS, &success);
	if (success != GL_TRUE)
	{
		glGetShaderInfoLog (m_vertex_id, 512, NULL, infoLog);
		Q2_Assert (false, infoLog);
	}

	glAttachShader (m_program_id, m_vertex_id);
	glAttachShader (m_program_id, m_fragment_id);
	glLinkProgram (m_program_id);

	GLint result{0};
	glGetProgramiv (m_program_id, GL_LINK_STATUS, &result);
	if (GL_FALSE == result)
	{
		GLchar eLog[1024] = {0};
		glGetProgramInfoLog (m_program_id, sizeof (eLog), nullptr, eLog);
		Q2_Assert (false, "Link error: %s", eLog);
	}

	set_name (in_name);
}

void Shader::set_name (const char *in_name)
{
	Q2_Assert (in_name != nullptr && in_name[0] != '\0', "EMPTY NAME!");
	m_name = in_name;

	char buffer[512u];
	sprintf (buffer, "Shader Program: %s", in_name);
	glObjectLabel (GL_PROGRAM, m_program_id, -1, buffer);

	sprintf (buffer, "Vertex Shader: %s", m_fragment_shader_path.c_str ());
	glObjectLabel (GL_SHADER, m_vertex_id, -1, buffer);

	sprintf (buffer, "Fragment Shader: %s", m_fragment_shader_path.c_str ());
	glObjectLabel (GL_SHADER, m_fragment_id, -1, buffer);
}

void Shader::use ()
{
	glUseProgram (m_program_id);
}

void Shader::set_uniform_1i (const std::string &name, std::int32_t value) const
{
	glUseProgram (m_program_id);

	const std::int32_t location = glGetUniformLocation (m_program_id, name.c_str ());
	if (location >= 0)
		glUniform1i (location, value);
}

void Shader::set_uniform_mat4 (const std::string &name, const glm::mat4 &value) const
{
	glUseProgram (m_program_id);

	const std::int32_t location = glGetUniformLocation (m_program_id, name.c_str ());
	if (location >= 0)
		glUniformMatrix4fv (location, 1, GL_FALSE, glm::value_ptr (value));
}

Shader::~Shader ()
{
}
}  // namespace Q2
