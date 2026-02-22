#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;      // base texture UV
layout (location = 2) in vec2 aLightmapCoord; // lightmap UV

out vec2 TexCoord;
out vec2 LightmapCoord;

uniform mat4 uMVP;

void main()
{
    gl_Position = uMVP * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
    LightmapCoord = aLightmapCoord;
}