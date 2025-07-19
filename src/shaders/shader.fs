#version 330 core
out vec4 FragColor;

in vec3 vertexPos;
in vec3 vertexColor;
in vec2 TexCoord;

uniform sampler2D boxTexture;
uniform sampler2D epicTexture;

void main()
{
    FragColor = mix(texture(boxTexture, TexCoord), texture(epicTexture, TexCoord), 0.2);
};
