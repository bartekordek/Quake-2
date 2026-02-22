#version 330 core

in vec2 TexCoord;
in vec2 LightmapCoord;

out vec4 FragColor;

uniform sampler2D uBaseTexture;
uniform sampler2D uLightmap;

void main()
{
    vec4 baseColor = texture(uBaseTexture, TexCoord);
    vec3 light = texture(uLightmap, LightmapCoord).rgb;

    // Quake 2 style modulation
    vec3 finalColor = baseColor.rgb * light;

    FragColor = vec4(finalColor, baseColor.a);
}