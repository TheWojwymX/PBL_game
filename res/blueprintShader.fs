#version 430 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 additionalColor;

void main()
{
    vec3 finalColor = additionalColor;

    FragColor = vec4(finalColor, 0.5);
}