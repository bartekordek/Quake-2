#pragma once

#include "shared/noncopyable.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace Q2
{
class Camera
{
public:
	Camera ()  = default;

	void rotate (float angle, float x, float y, float z);
	void translate (float x, float y, float z);

	~Camera () = default;

	Q2_NONCOPYABLE (Camera)

protected:
private:
	glm::mat4 m_view_matrix = glm::mat4 (1.0f);
};
}  // namespace Q2