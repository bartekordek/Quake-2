#include <GL/glew.h>
#include "ref_gl/polygon.hpp"
#include "ref_gl/gl_model.h"
#include "ref_gl/gl_surf.h"
#include "ref_gl/gl_local.h"
#include "ref_gl/gl_light.h"
#include "ref_gl/gl_state.hpp"
#include "ref_gl/attribute_meta.hpp"
#include "ref_gl/lightmapstate.hpp"
#include "ref_gl/shader.hpp"

#include "shared/assert.h"


#define GL_LIGHTMAP_FORMAT GL_RGBA

namespace Q2
{

PolygonStore &PolygonStore::get_instance ()
{
	static PolygonStore instance;
	return instance;
}

Polygon *PolygonStore::get_or_create (msurface_s *surface_info)
{
	auto it = m_polygons.find (surface_info->texinfo->image->name);
	if (it != m_polygons.end ())
	{
		return it->second;
	}

	Polygon *result								   = new Polygon (surface_info);
	image_t *image								   = R_TextureAnimation (surface_info->texinfo);
	m_polygons[surface_info->texinfo->image->name] = result;

	return result;
}

Polygon::Polygon ()
{
}

Polygon::Polygon (msurface_s *in_surface) : m_surface_info (in_surface)
{
	if (m_surface_info->polys)
	{
		glpoly_s *p = m_surface_info->polys;
		glGenVertexArrays (1, &m_polygon_data.m_vao);
		glGenBuffers (1, &m_polygon_data.m_vbo);

		glBindVertexArray (m_polygon_data.m_vao);
		const std::uint32_t vertex_size = VERTEXSIZE;

		std::vector<float> vertex_data;
		float			  *v = p->verts[0];
		for (std::size_t i = 1u; i < p->numverts - 1u; ++i, v += vertex_size)
		{
			for (std::size_t j = 0u; j < 5; ++j)
			{
				vertex_data.push_back (v[j]);
			}
		}

		std::vector<float> vertex_trianulated;
		const std::size_t  vertex_count = vertex_data.size ();
		m_triangles_count				= 0u;
		for (std::size_t i = 1u; i < vertex_count - 1u; ++i)
		{
			vertex_trianulated.push_back (vertex_data[0]);
			vertex_trianulated.push_back (vertex_data[i]);
			vertex_trianulated.push_back (vertex_data[i + 1]);
			++m_triangles_count;
		}

		glBufferData (GL_ARRAY_BUFFER, sizeof (vertex_trianulated), vertex_trianulated.data (), GL_STATIC_DRAW);

		std::vector<AttributeMeta> attributeInfo;
		attributeInfo.push_back (AttributeMeta ("pos", 0, 3, (int) GL_FLOAT, false, vertex_size * sizeof (float), nullptr));
		attributeInfo.push_back (AttributeMeta ("uv", 1, 2, (int) GL_FLOAT, false, vertex_size * sizeof (float), (void *) (3 * sizeof (float))));

		for (const auto &att : attributeInfo)
		{
			glVertexAttribPointer (att.index, att.size, att.type, att.normalized, att.stride, att.pointer);
			glEnableVertexAttribArray (att.index);
		}
	}

	m_shader = std::make_unique<Shader> ();
	m_shader->init ("../shaders/basic_texture.vert", "../shaders/basic_texture.frag");
}

void Polygon::set_name (const char *in_name)
{
	m_name = in_name;

	char buffer[256u];

	if (m_image->texnum)
	{
		glBindTexture (GL_TEXTURE_2D, m_image->texnum);

		sprintf (buffer, "Texture: %s", in_name);
		glObjectLabel (GL_TEXTURE, m_image->texnum, -1, buffer);
	}

	sprintf (buffer, "VAO: %s", in_name);
	glObjectLabel (GL_VERTEX_ARRAY, m_polygon_data.m_vao, -1, buffer);

	sprintf (buffer, "VBO: %s", in_name);
	glObjectLabel (GL_BUFFER, m_polygon_data.m_vbo, -1, buffer);

	sprintf (buffer, "Shader: %s", in_name);
	m_shader->set_name (buffer);
}

void Polygon::draw ()
{
	auto &gl_state		= Q2::glstate_t::get_instance ();
	auto &gl_lms		= Q2::Light_map_state::get_instance ();

	qboolean is_dynamic = e_false;

	auto &fa			= m_surface_info;

	m_image				= R_TextureAnimation (fa->texinfo);

	glBindTexture (GL_TEXTURE_2D, fa->texinfo->image->texnum);

	if (fa->texinfo->flags & SURF_DRAWTURB)
	{
		return;
	}

	//======
	// PGM
	if (fa->texinfo->flags & SURF_FLOWING)
	{
		DrawGLFlowingPoly (fa);
	}
	else
	{
		DrawGLPoly (fa->polys);
	}
	// PGM
	//======

	/*
	** check for lightmap modification
	*/
	std::size_t maps = 0u;
	for (maps = 0u; maps < MAXLIGHTMAPS && fa->styles[maps] != 255; maps++)
	{
		if (r_newrefdef.lightstyles[fa->styles[maps]].white != fa->cached_light[maps])
			goto dynamic;
	}

	// dynamic this frame or dynamic previously
	if ((fa->dlightframe == r_framecount))
	{
	dynamic:
		if (gl_dynamic->value)
		{
			if (!(fa->texinfo->flags & (SURF_SKY | SURF_TRANS33 | SURF_TRANS66 | SURF_WARP)))
			{
				is_dynamic = e_true;
			}
		}
	}

	if (is_dynamic)
	{
		if ((fa->styles[maps] >= 32 || fa->styles[maps] == 0) && (fa->dlightframe != r_framecount))
		{
			unsigned temp[34 * 34];
			int		 smax, tmax;

			smax = (fa->extents[0] >> 4) + 1;
			tmax = (fa->extents[1] >> 4) + 1;

			R_BuildLightMap (fa, (byte *) temp, smax * 4);
			R_SetCacheState (fa);

			GL_BindTexture (gl_state.lightmap_textures[fa->lightmaptexturenum]);

			qglTexSubImage2D (GL_TEXTURE_2D, 0, fa->light_s, fa->light_t, smax, tmax, GL_LIGHTMAP_FORMAT, GL_UNSIGNED_BYTE, temp);

			fa->lightmapchain								 = gl_lms.lightmap_surfaces[fa->lightmaptexturenum];
			gl_lms.lightmap_surfaces[fa->lightmaptexturenum] = fa;
		}
		else
		{
			fa->lightmapchain			= gl_lms.lightmap_surfaces[0];
			gl_lms.lightmap_surfaces[0] = fa;
		}
	}
	else
	{
		fa->lightmapchain								 = gl_lms.lightmap_surfaces[fa->lightmaptexturenum];
		gl_lms.lightmap_surfaces[fa->lightmaptexturenum] = fa;
	}
}

void Polygon::draw_surface (msurface_s *surface_info, std::uint32_t tex_id)
{
	// TODO: implement this function
	//GL_BindTexture (tex_id);
	//msurface_s *surf = surface_info;
	//for (; surf != 0; surf = surf->lightmapchain)
	//{
	//	if (surf->polys)
	//	{
	//		DrawGLPolyChain (surf->polys, 0, 0);
	//	}
	//}
}

void Polygon::DrawGLFlowingPoly (msurface_s *fa)
{
	//int		  i;
	//float	 *v;
	//glpoly_t *p;
	//float	  scroll;

	//p	   = fa->polys;

	//scroll = -64 * ((r_newrefdef.time / 40.0) - (int) (r_newrefdef.time / 40.0));
	//if (scroll == 0.0)
	//	scroll = -64.0;

	//qglBegin (GL_POLYGON);
	//v = p->verts[0];
	//for (i = 0; i < p->numverts; i++, v += VERTEXSIZE)
	//{
	//	qglTexCoord2f ((v[3] + scroll), v[4]);
	//	qglVertex3fv (v);
	//}
	//qglEnd ();
}

void Polygon::DrawGLPoly (glpoly_s *p)
{
	//int	   i;
	//float *v;

	//qglBegin (GL_POLYGON);
	//v = p->verts[0];
	//for (i = 0; i < p->numverts; i++, v += VERTEXSIZE)
	//{
	//	qglTexCoord2f (v[3], v[4]);
	//	qglVertex3fv (v);
	//}
	//qglEnd ();

	m_shader->use ();
	glBindVertexArray (m_polygon_data.m_vao);
	glDrawArrays (GL_TRIANGLES, 0, m_triangles_count);
}

}  // namespace Q2