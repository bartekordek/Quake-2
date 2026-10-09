#version 330 core

in vec2 TexCoord;
in vec4 VertColor;

out vec4 FragColor;

uniform sampler2D uTexture;
uniform int uUseTexture;

void main()
{
    if (uUseTexture != 0)
        FragColor = texture(uTexture, TexCoord) * VertColor;
    else
        FragColor = VertColor;

    if (FragColor.a < 0.01)
        discard;
}
