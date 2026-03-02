#include "gl_modern/lightmapstate.hpp"

namespace Q2
{
Light_map_state &Light_map_state::get_instance ()
{
	static Light_map_state instance;
	return instance;
}
}  // namespace Q2