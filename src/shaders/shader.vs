#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;

out vec3 vertexPos;
out vec3 vertexColor;
out vec2 TexCoord;

void main()
{
    vertexPos = aPos;
    gl_Position = vec4(vertexPos, 1.0);
    vertexColor = aColor;
    TexCoord = aTexCoord;
};
