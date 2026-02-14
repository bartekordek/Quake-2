#pragma once

#include "shared/noncopyable.hpp"
#include <string>
#include <cstdint>

namespace Q2
{
	class Shader
	{
	public:
		Shader ();

		void init (const char *vertex_shader, const char *fragment_shader);
		void use ();
		void set_uniform_1i (const std::string &name, std::int32_t value) const;
		~Shader ();

		Q2_NONCOPYABLE (Shader)
	protected:
	private:
		std::uint32_t m_program_id{0};
		std::uint32_t m_vertex_id{0};
		std::uint32_t m_fragment_id{0};
	};
}
