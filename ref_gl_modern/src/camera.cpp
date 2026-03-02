#include "gl_modern/camera.hpp"
#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>

namespace Q2
{
void Camera::rotate (float angle, float x, float y, float z)
{
	glm::vec3 axis (x, y, z);
	axis			   = glm::normalize (axis);

	glm::mat4 rotation = glm::rotate (glm::mat4 (1.0f), glm::radians (angle), axis);

	// Match legacy OpenGL behavior:
	m_view_matrix	   = m_view_matrix * rotation;
}
void Camera::translate (float x, float y, float z)
{
	const glm::vec3 delta (x, y, z);

	glm::mat4 translation = glm::translate (glm::mat4 (1.0f), delta);

	m_view_matrix		  = m_view_matrix * translation;
}
}  // namespace Q2