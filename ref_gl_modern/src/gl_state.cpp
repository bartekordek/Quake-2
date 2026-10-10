#include "gl_modern/gl_state.hpp"

namespace Q2
{
glstate_t &glstate_t::get_instance ()
{
	static glstate_t instance;
	return instance;
}

glstate_t::glstate_t ()
{
}

glstate_t::~glstate_t ()
{
}

}  // namespace Q2

extern "C" float GL_InverseIntensity (void)
{
	return Q2::glstate_t::get_instance ().inverse_intensity;
}