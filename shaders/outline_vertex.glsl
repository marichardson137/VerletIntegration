#version 330 core

layout (location = 0) in vec3 vertexPos;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 vertexTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float outline;

out vec3 fragmentPos;
out vec3 fragmentVertexNormal;
out vec2 fragmentTexCoord;

void main()
{
    gl_Position = projection * view * model * vec4(vertexPos + vertexNormal * outline, 1.0);
}