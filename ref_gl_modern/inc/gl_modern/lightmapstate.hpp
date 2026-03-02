#pragma once

#include "shared/defines.h"
#include "shared/noncopyable.hpp"

typedef struct msurface_s;

namespace Q2
{
class Light_map_state
{
public:
	int			internal_format;
	int			current_lightmap_texture;
	msurface_s *lightmap_surfaces[MAX_LIGHTMAPS];
	int			allocated[BLOCK_WIDTH];
	// the lightmap texture data needs to be kept in
	// main memory so texsubimage can update properly
	byte lightmap_buffer[4 * BLOCK_WIDTH * BLOCK_HEIGHT];

	static Light_map_state &get_instance ();

	Q2_NONCOPYABLE (Light_map_state)
protected:
private:
	Light_map_state () = default;
	~Light_map_state () = default;

};
}  // namespace Q2