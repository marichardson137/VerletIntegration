#version 330 core

in vec3 fragmentPos;
in vec3 fragmentVertexNormal;
in vec2 fragmentTexCoord;

out vec4 color;

uniform sampler2D imageTexture;

void main()
{
    vec3 lightColor = vec3(1.0, 1.0, 1.0);
    vec3 lightPos = vec3(10.0, 10.0, 10.0);
    vec3 objectColor = vec3(0.6, 0.3, 0.7);

    // ambient
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse
    vec3 norm = normalize(fragmentVertexNormal);
    vec3 lightDir = normalize(lightPos - fragmentPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;


    vec3 result = (ambient + diffuse) * objectColor;

    color = vec4(result, 1.0);
    // color = texture(imageTexture, fragmentTexCoord);
}