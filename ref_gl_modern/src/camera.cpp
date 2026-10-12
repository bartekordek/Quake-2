#include "gl_modern/camera.hpp"
#include <cmath>

namespace Q2
{
Camera &Camera::get_instance ()
{
	static Camera instance;
	return instance;
}

void Camera::reset_view ()
{
	m_view_matrix = glm::mat4 (1.0f);
}

void Camera::set_perspective (float fovy_degrees, float aspect, float z_near, float z_far, float camera_separation)
{
	// Match MYgluPerspective / classic Quake frustum
	const float ymax = z_near * tanf (glm::radians (fovy_degrees) * 0.5f);
	const float ymin = -ymax;
	float		xmin = ymin * aspect;
	float		xmax = ymax * aspect;

	xmin += -(2.f * camera_separation) / z_near;
	xmax += -(2.f * camera_separation) / z_near;

	m_proj_matrix = glm::frustum (xmin, xmax, ymin, ymax, z_near, z_far);
}

void Camera::rotate (float angle, float x, float y, float z)
{
	glm::vec3 axis (x, y, z);
	axis			   = glm::normalize (axis);

	glm::mat4 rotation = glm::rotate (glm::mat4 (1.0f), glm::radians (angle), axis);

	// Match legacy OpenGL glRotate behavior (post-multiply on modelview)
	m_view_matrix	   = m_view_matrix * rotation;
}

void Camera::translate (float x, float y, float z)
{
	const glm::vec3 delta (x, y, z);

	glm::mat4 translation = glm::translate (glm::mat4 (1.0f), delta);

	m_view_matrix		  = m_view_matrix * translation;
}
}  // namespace Q2
