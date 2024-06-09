#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aInstancePos;

uniform mat4 lightSpaceMatrix;

void main()
{
    vec3 worldPos = aPos + aInstancePos;
    gl_Position = lightSpaceMatrix * vec4(worldPos, 1.0);
}