#pragma once

#include "shared/noncopyable.hpp"
#include <glm/mat4x4.hpp>
#include <cstddef>
#include <cstdint>
#include <string>
#include <memory>
#include <unordered_map>

namespace Q2
{
class Shader;

struct ShaderKey
{
	std::string vertex;
	std::string fragment;

	bool operator== (const ShaderKey &other) const;
	bool operator< (const ShaderKey &other) const;
};

struct ShaderKeyHash
{
	std::size_t operator() (const ShaderKey &key) const noexcept;
};

class ShaderStore
{
public:
	static ShaderStore &get_instance ();

	Shader* get_or_create_shader (const ShaderKey &key);
protected:
private:
	ShaderStore ();
	~ShaderStore ();

	std::unordered_map<ShaderKey, std::unique_ptr<Shader>, ShaderKeyHash> m_shaders;
};

class Shader
{
public:
	Shader ();

	void init (const char *vertex_shader, const char *fragment_shader, const char *in_name);
	void use ();
	void set_uniform_1i (const std::string &name, std::int32_t value) const;
	void set_uniform_mat4 (const std::string &name, const glm::mat4 &value) const;
	void set_name (const char *in_name);
	~Shader ();

	Q2_NONCOPYABLE (Shader)
protected:
private:
	std::string	  m_name;
	std::string	  m_fragment_shader_path;
	std::string	  m_vertex_shader_path;
	std::uint32_t m_program_id{0};
	std::uint32_t m_vertex_id{0};
	std::uint32_t m_fragment_id{0};
};
}  // namespace Q2
