#pragma once

#include "shared/noncopyable.hpp"
#include "shared/defines.h"
#include <array>
#include <cstdint>

namespace Q2
{
class glstate_t
{
public:
	static glstate_t &get_instance ();

	float inverse_intensity;
	bool  fullscreen;

	int prev_mode;

	unsigned char *d_16to8table;

	std::array<std::uint32_t, TEXNUM_LIGHTMAPS> lightmap_textures;

	int currenttextures[2];
	int currenttmu;

	float camera_separation;
	bool  stereo_enabled;

	unsigned char originalRedGammaTable[256];
	unsigned char originalGreenGammaTable[256];
	unsigned char originalBlueGammaTable[256];

	Q2_NONCOPYABLE (glstate_t);

protected:
private:
	glstate_t ();
	~glstate_t ();
};
}  // namespace Q2