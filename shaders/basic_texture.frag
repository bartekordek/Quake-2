#version 330 core

in vec3 VertColor;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D tex;

// 0: RGBA, 1: RGBM
uniform int mode;

vec4 decodeRGBM(vec4 rgbm)
{
    float maxRange = 8.0;
    return rgbm * (rgbm.a * maxRange);
}

void main()
{
    vec4 color;
    if (mode == 1)
        color = decodeRGBM(texture(tex, TexCoord));
    else
        color = texture(tex, TexCoord);

    // Quake GL_ALPHA_TEST equivalent — index 255 / low alpha is transparent
    if (color.a < 0.666)
        discard;

    FragColor = color;
}
