#version 330 core

layout (location = 0) in vec3 vertexPos;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 vertexTexCoord;
layout (location = 3) in vec3 instancePosition;
layout (location = 4) in float instanceVelocity;

uniform float scale;
uniform mat4 view;
uniform mat4 projection;

out vec3 fragmentPos;
out vec3 fragmentVertexNormal;
out float fragmentVelocity;

mat4 translationMatrix(vec3 translation)
{
    return mat4(
        vec4(scale, 0.0, 0.0, 0.0),
        vec4(0.0, scale, 0.0, 0.0),
        vec4(0.0, 0.0, scale, 0.0),
        vec4(translation, scale));
}

void main()
{
    mat4 model = translationMatrix(instancePosition);
    fragmentPos = vec3(model * vec4(vertexPos, 1.0));
    fragmentVertexNormal = mat3(transpose(inverse(model))) * vertexNormal;
    fragmentVelocity = instanceVelocity;

    gl_Position = projection * view * vec4(fragmentPos, 1.0);
}