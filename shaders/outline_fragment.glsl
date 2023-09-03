#version 330 core

in vec3 fragmentVertexNormal;
in vec2 fragmentTexCoord;

out vec4 color;

float near = 0.1;
float far = 100.0;

float LinearizeDepth(float depth);

void main()
{
    float depth = LinearizeDepth(gl_FragCoord.z) / far;
    color = vec4(1.0, 1.0, 1.0, depth * 4);
}

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // back to NDC
    return (2.0 * near * far) / (far + near - z * (far - near));
}
