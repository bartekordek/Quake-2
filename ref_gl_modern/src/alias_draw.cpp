#include <GL/glew.h>
#include "gl_modern/alias_draw.hpp"
#include "gl_modern/camera.hpp"
#include "gl_modern/shader.hpp"
#include "gl_modern/gl_local.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstring>
#include <memory>

namespace
{
std::unique_ptr<Q2::Shader> g_alias_shader;
GLuint						g_alias_vao = 0;
GLuint						g_alias_vbo = 0;

void ensure_alias_resources ()
{
	if (g_alias_shader)
		return;

	g_alias_shader = std::make_unique<Q2::Shader> ();
	g_alias_shader->init ("../shaders/alias.vert", "../shaders/alias.frag");

	glGenVertexArrays (1, &g_alias_vao);
	glGenBuffers (1, &g_alias_vbo);

	glBindVertexArray (g_alias_vao);
	glBindBuffer (GL_ARRAY_BUFFER, g_alias_vbo);

	const GLsizei stride = 9 * sizeof (float);
	glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void *> (0));
	glEnableVertexAttribArray (0);
	glVertexAttribPointer (1, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void *> (3 * sizeof (float)));
	glEnableVertexAttribArray (1);
	glVertexAttribPointer (2, 4, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void *> (5 * sizeof (float)));
	glEnableVertexAttribArray (2);

	glBindVertexArray (0);
}

glm::mat4 build_entity_model_matrix (const entity_t *e)
{
	// Match R_RotateForEntity (with caller's pitch negation already applied on e->angles[PITCH])
	glm::mat4 m (1.0f);
	m = glm::translate (m, glm::vec3 (e->origin[0], e->origin[1], e->origin[2]));
	m = glm::rotate (m, glm::radians (e->angles[1]), glm::vec3 (0.f, 0.f, 1.f));
	m = glm::rotate (m, glm::radians (-e->angles[0]), glm::vec3 (0.f, 1.f, 0.f));
	m = glm::rotate (m, glm::radians (-e->angles[2]), glm::vec3 (1.f, 0.f, 0.f));
	return m;
}
}  // namespace

extern "C" void Alias_DrawTriangles (const float *interleaved9, int vertex_count, unsigned int skin_texnum, const float *mvp16,
									 int use_texture)
{
	if (!interleaved9 || vertex_count <= 0)
		return;

	ensure_alias_resources ();

	g_alias_shader->use ();
	g_alias_shader->set_uniform_mat4 ("uMVP", glm::make_mat4 (mvp16));
	g_alias_shader->set_uniform_1i ("uTexture", 0);
	g_alias_shader->set_uniform_1i ("uUseTexture", use_texture ? 1 : 0);

	glActiveTexture (GL_TEXTURE0);
	glBindTexture (GL_TEXTURE_2D, skin_texnum);

	glBindVertexArray (g_alias_vao);
	glBindBuffer (GL_ARRAY_BUFFER, g_alias_vbo);
	glBufferData (GL_ARRAY_BUFFER, static_cast<GLsizeiptr> (vertex_count * 9 * sizeof (float)), interleaved9, GL_STREAM_DRAW);
	glDrawArrays (GL_TRIANGLES, 0, vertex_count);
	glBindVertexArray (0);
}

extern "C" void R_DrawSkyQuad (unsigned int texnum, float rotate_deg, const float axis[3], const float origin[3],
							  const float verts[4][5])
{
	float tri[6 * 9];
	int	  i;
	/* quad 0,1,2,3 -> triangles 0,1,2 and 0,2,3 */
	const int idx[6] = {0, 1, 2, 0, 2, 3};

	auto	 &camera = Q2::Camera::get_instance ();
	glm::mat4 model (1.0f);
	model = glm::translate (model, glm::vec3 (origin[0], origin[1], origin[2]));
	if (rotate_deg != 0.f)
	{
		glm::vec3 ax (axis[0], axis[1], axis[2]);
		if (glm::dot (ax, ax) > 0.0001f)
			model = glm::rotate (model, glm::radians (rotate_deg), glm::normalize (ax));
	}
	const glm::mat4 mvp = camera.projection () * camera.view () * model;
	float			mvp16[16];
	memcpy (mvp16, glm::value_ptr (mvp), sizeof (mvp16));

	for (i = 0; i < 6; i++)
	{
		const float *v	= verts[idx[i]];
		float		*d	= tri + i * 9;
		d[0]			= v[0];
		d[1]			= v[1];
		d[2]			= v[2];
		d[3]			= v[3];
		d[4]			= v[4];
		d[5]			= d[6] = d[7] = d[8] = 1.f;
	}

	const GLboolean cull = glIsEnabled (GL_CULL_FACE);
	glDisable (GL_CULL_FACE);
	glDepthMask (GL_FALSE);
	Alias_DrawTriangles (tri, 6, texnum, mvp16, 1);
	glDepthMask (GL_TRUE);
	if (cull)
		glEnable (GL_CULL_FACE);
}

extern "C" void R_DrawCameraTriangles (const float *interleaved9, int vertex_count, unsigned int texnum, int blend,
									   int depth_write)
{
	if (!interleaved9 || vertex_count <= 0)
		return;

	float mvp16[16];
	const glm::mat4 mvp = Q2::Camera::get_instance ().mvp ();
	memcpy (mvp16, glm::value_ptr (mvp), sizeof (mvp16));

	if (blend)
	{
		glEnable (GL_BLEND);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	glDepthMask (depth_write ? GL_TRUE : GL_FALSE);

	Alias_DrawTriangles (interleaved9, vertex_count, texnum, mvp16, 1);

	glDepthMask (GL_TRUE);
	if (blend)
		glDisable (GL_BLEND);
}

extern "C" void Alias_ComputeMVP (const entity_t *e, int lefthand_mirror, float *out_mvp16)
{
	auto	   &camera = Q2::Camera::get_instance ();
	glm::mat4	proj   = camera.projection ();
	if (lefthand_mirror)
		proj = glm::scale (proj, glm::vec3 (-1.f, 1.f, 1.f));

	const glm::mat4 model = build_entity_model_matrix (e);
	const glm::mat4 mvp	  = proj * camera.view () * model;
	memcpy (out_mvp16, glm::value_ptr (mvp), 16 * sizeof (float));
}
