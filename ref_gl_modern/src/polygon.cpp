#include <GL/glew.h>
#include "gl_modern/polygon.hpp"
#include "gl_modern/gl_model.h"
#include "gl_modern/gl_surf.h"
#include "gl_modern/gl_local.h"
#include "gl_modern/gl_light.h"
#include "gl_modern/gl_state.hpp"
#include "gl_modern/attribute_meta.hpp"
#include "gl_modern/lightmapstate.hpp"
#include "gl_modern/shader.hpp"
#include "gl_modern/camera.hpp"

#include "shared/assert.h"
#include <vector>

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
	auto it = m_polygons.find (surface_info);
	if (it != m_polygons.end ())
	{
		return it->second;
	}

	Polygon *result		  = new Polygon (surface_info);
	m_polygons[surface_info] = result;

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
		glBindBuffer (GL_ARRAY_BUFFER, m_polygon_data.m_vbo);

		constexpr std::uint32_t vertex_size = VERTEXSIZE;

		// Fan-triangulate: (0, i, i+1) for i in [1, numverts-2]
		std::vector<float> vertex_triangulated;
		float			  *verts = p->verts[0];
		m_triangles_count		 = 0u;

		for (int i = 1; i < p->numverts - 1; ++i)
		{
			const float *v0 = verts;
			const float *v1 = verts + i * vertex_size;
			const float *v2 = verts + (i + 1) * vertex_size;

			for (std::uint32_t j = 0; j < vertex_size; ++j) vertex_triangulated.push_back (v0[j]);
			for (std::uint32_t j = 0; j < vertex_size; ++j) vertex_triangulated.push_back (v1[j]);
			for (std::uint32_t j = 0; j < vertex_size; ++j) vertex_triangulated.push_back (v2[j]);
			m_triangles_count += 3u;
		}

		glBufferData (GL_ARRAY_BUFFER, vertex_triangulated.size () * sizeof (float), vertex_triangulated.data (), GL_STATIC_DRAW);

		const GLsizei stride = static_cast<GLsizei> (vertex_size * sizeof (float));
		glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void *> (0));
		glEnableVertexAttribArray (0);
		glVertexAttribPointer (1, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void *> (3 * sizeof (float)));
		glEnableVertexAttribArray (1);
		glVertexAttribPointer (2, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void *> (5 * sizeof (float)));
		glEnableVertexAttribArray (2);

		glBindVertexArray (0);

		m_texId = m_surface_info->texinfo->image->texnum;
	}

	m_shader = std::make_unique<Shader> ();
	m_shader->init ("../shaders/lightmap.vert", "../shaders/lightmap.frag");
}

void Polygon::set_name (const char *in_name)
{
	m_name = in_name;

	char buffer[256u];

	if (m_image && m_image->texnum)
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
	draw_impl (m_texId);
}

void Polygon::draw (std::uint32_t in_tex_id)
{
	draw_impl (in_tex_id);
}

void Polygon::draw_impl (std::uint32_t in_tex_id)
{
	IRenderable::draw ();

	auto &gl_state = Q2::glstate_t::get_instance ();
	auto &gl_lms   = Q2::Light_map_state::get_instance ();

	qboolean is_dynamic = e_false;

	auto &fa = m_surface_info;

	m_image	 = R_TextureAnimation (fa->texinfo);
	in_tex_id = m_image ? m_image->texnum : in_tex_id;

	if (fa->flags & SURF_DRAWTURB)
	{
		EmitWaterPolys (fa);
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
		if (maps < MAXLIGHTMAPS && (fa->styles[maps] >= 32 || fa->styles[maps] == 0) && (fa->dlightframe != r_framecount))
		{
			unsigned temp[128 * 128];
			int		 smax, tmax;

			smax = (fa->extents[0] >> 4) + 1;
			tmax = (fa->extents[1] >> 4) + 1;

			R_BuildLightMap (fa, (byte *) temp, smax * 4);
			R_SetCacheState (fa);

			// DrawGLPoly binds via glBindTexture and bypasses GL_BindTexture's cache —
			// force the lightmap onto unit 0 before TexSubImage.
			glActiveTexture (GL_TEXTURE0);
			gl_state.currenttextures[0] = -1;
			gl_state.currenttmu			= 0;
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

void Polygon::overwerite_tex_id (std::uint32_t tex_id)
{
	m_texId = tex_id;
}

void Polygon::draw_surface (msurface_s *surface_info, std::uint32_t tex_id)
{
	GL_BindTexture (tex_id);
}

void Polygon::DrawGLFlowingPoly (msurface_s *fa)
{
	// Flowing UVs not yet updated per-frame in the VBO; draw static for now.
	DrawGLPoly (fa->polys);
}

void Polygon::DrawGLPoly (glpoly_s *p)
{
	if (!p || m_triangles_count == 0 || !m_shader)
		return;

	auto	   &gl_state = glstate_t::get_instance ();
	const auto &camera	 = Camera::get_instance ();

	const std::uint32_t base_tex = m_image ? m_image->texnum : m_texId;
	const std::uint32_t lm_tex	 = gl_state.lightmap_textures[m_surface_info->lightmaptexturenum];

	m_shader->use ();
	m_shader->set_uniform_mat4 ("uMVP", camera.mvp ());
	m_shader->set_uniform_1i ("uBaseTexture", 0);
	m_shader->set_uniform_1i ("uLightmap", 1);

	glActiveTexture (GL_TEXTURE0);
	glBindTexture (GL_TEXTURE_2D, base_tex);
	gl_state.currenttextures[0] = static_cast<int> (base_tex);

	glActiveTexture (GL_TEXTURE1);
	glBindTexture (GL_TEXTURE_2D, lm_tex);
	gl_state.currenttextures[1] = static_cast<int> (lm_tex);

	glBindVertexArray (m_polygon_data.m_vao);
	glDrawArrays (GL_TRIANGLES, 0, static_cast<GLsizei> (m_triangles_count));
	glBindVertexArray (0);

	glActiveTexture (GL_TEXTURE0);
	gl_state.currenttmu = 0;
}

}  // namespace Q2
