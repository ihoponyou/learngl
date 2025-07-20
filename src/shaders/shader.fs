#version 330 core
out vec4 FragColor;

in vec3 vertexPos;
in vec3 vertexColor;
in vec2 TexCoord;

uniform sampler2D boxTexture;
uniform sampler2D epicTexture;
uniform float alpha;

void main()
{
    FragColor = mix(texture(boxTexture, TexCoord), texture(epicTexture, vec2(-TexCoord.x, TexCoord.y)), alpha);
};
