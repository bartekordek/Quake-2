#include "ref_gl/utils.hpp"
#include "ref_gl/gl_local.h"

EXTERNC viddef_t vid;

namespace Q2
{

float to_x_normalized (float in_x)
{
	return (in_x * 2.f / static_cast<float> (vid.width)) - 1.f;
}

float to_y_normalized (float in_y)
{
	return 1.f - 2.f * in_y / static_cast<float> (vid.height);
}

float to_x_normalized(float in_x, float width)
{
	return (in_x * 2.f / static_cast<float> (width)) - 1.f;
}

float to_y_normalized(float in_y, float height)
{
	return 1.f - 2.f * in_y / static_cast<float> (height);
}

float width_to_normalized (float in_width)
{
	return 2.f * in_width / static_cast<float> (vid.width);
}

float height_to_normalized (float in_height)
{
	return 2.f * in_height / static_cast<float> (vid.height);
}

}