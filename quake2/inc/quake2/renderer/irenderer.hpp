#pragma once

#include "shared/ref.h"
#include <cstdint>

enum class RendererType: std::uint8_t
{
	Software,
	OpenGL,
	OpenGLModern,
	Direct3D
};

class IRenderer
{
public:
protected:
private:
};


refexport_t CreateRefAPI (RendererType in_type, refimport_t rimp);