#pragma once

#include "shared/noncopyable.hpp"
#include <string>
#include <cstdint>
#include <array>
#include <memory>

namespace Q2
{
class Shader;

struct AttributeMeta
{
	std::string	 name;
	int			 index		= 0;
	int			 size		= 0;
	unsigned	 type		= 0;
	bool		 normalized = false;
	int			 stride		= 0;
	void		*pointer	= nullptr;

	AttributeMeta (const std::string &inName, int inIndex, int inSize, int inType, bool inNormalized, int inStride, void *inPointer)
		: name (inName), index (inIndex), size (inSize), type (inType), normalized (inNormalized), stride (inStride), pointer (inPointer)
	{
	}
};

	struct RenderData
	{
		std::int32_t id{0};

		std::int32_t x{0};
		std::int32_t y{0};
		std::int32_t w{0};
		std::int32_t h{0};

		std::int32_t imgW{0};
		std::int32_t imgH{0};

		float scale{1.f};

		bool alphaTest{false};
		std::int32_t data_type{0};
		std::int32_t format{0};
		std::int32_t internal_format{0};

		void *data{nullptr};
	};

	class Texture
	{
	public:
		Texture ();

		void		init ();
		void		draw (const RenderData &inData);
		static void drawStatic (const RenderData &inData);
		void		changeScale (float in_scale);

		~Texture ();

		Q2_NONCOPYABLE (Texture)
	protected:
	private:
		void					initBuffers ();
		std::uint32_t			m_vao{0u};
		std::uint32_t			m_vbo{0u};
		std::uint32_t			m_ebo{0u};
		std::unique_ptr<Shader> m_shader;
		float					m_scale{0.9375f};
	};
	}
