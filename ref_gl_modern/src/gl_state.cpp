#include "ref_gl/gl_state.hpp"

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