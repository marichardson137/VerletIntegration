#version 330 core

layout (location = 0) in vec3 vertexPos;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 vertexTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 fragmentPos;
out vec3 fragmentVertexNormal;
out vec2 fragmentTexCoord;

void main()
{
    fragmentPos = vec3(model * vec4(vertexPos, 1.0));
    fragmentVertexNormal = mat3(transpose(inverse(model))) * vertexNormal;
    fragmentTexCoord = vertexTexCoord;

    gl_Position = projection * view * vec4(fragmentPos, 1.0);
}