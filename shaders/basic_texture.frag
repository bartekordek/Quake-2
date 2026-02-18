#version 330 core

out vec4 FragColor;

in vec2 TexCoord;
in vec3 VertColor;

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
    
    if( mode == 0) 
    {
        FragColor = texture(tex, TexCoord);
    }
    else if ( mode == 1 )
    {
        vec4 rgba_value = texture(tex, TexCoord);
        FragColor = decodeRGBM(rgba_value);
    }
}