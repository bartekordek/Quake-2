#pragma once


#include "gl_modern/i_renderable.hpp"
#include "shared/config.h"

#include <unordered_map>
#include <memory>
#include <string>

typedef struct msurface_s;
typedef struct image_s;
typedef struct glpoly_s;

namespace Q2
{
class Shader;

struct PolygonData
{
	std::uint32_t m_vao{0u};
	std::uint32_t m_vbo{0u};
	std::uint32_t TrianglesCount{0u};
};

class Polygon: public IRenderable
{
public:
	Polygon ();
	Polygon (msurface_s *in_surface);

	void		set_name (const char *in_name);
	void		draw () override;
	void		draw (std::uint32_t in_tex_id);
	void		overwerite_tex_id (std::uint32_t tex_id);
	static void draw_surface (msurface_s *surface_info, std::uint32_t tex_id);

	~Polygon () = default;

	Q2_NONCOPYABLE (Polygon)

protected:
private:
	void					draw_impl (std::uint32_t in_tex_id);
	void					DrawGLFlowingPoly (msurface_s *fa);
	void					DrawGLPoly (glpoly_s *p);
	std::uint32_t			m_texId{0u};
	std::string				m_name;
	msurface_s			   *m_surface_info{nullptr};
	image_s				   *m_image{nullptr};
	PolygonData				m_polygon_data;
	std::unique_ptr<Shader> m_shader;

	std::size_t m_triangles_count{0u};
};

class PolygonStore
{
public:
	static PolygonStore &get_instance ();
	Polygon				*get_or_create (msurface_s *surface_info);

protected:
private:
	PolygonStore ()	 = default;
	~PolygonStore () = default;

	std::unordered_map<std::string, Polygon*> m_polygons;
};

};	// namespace Q2
