#pragma once

#include "shared/noncopyable.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace Q2
{
class Camera
{
public:
	static Camera &get_instance ();

	void reset_view ();
	void set_perspective (float fovy_degrees, float aspect, float z_near, float z_far, float camera_separation = 0.f);

	void rotate (float angle, float x, float y, float z);
	void translate (float x, float y, float z);

	const glm::mat4 &view () const { return m_view_matrix; }
	const glm::mat4 &projection () const { return m_proj_matrix; }
	glm::mat4		 mvp () const { return m_proj_matrix * m_view_matrix; }

	~Camera () = default;

	Q2_NONCOPYABLE (Camera)

protected:
private:
	Camera () = default;

	glm::mat4 m_view_matrix = glm::mat4 (1.0f);
	glm::mat4 m_proj_matrix = glm::mat4 (1.0f);
};
}  // namespace Q2
