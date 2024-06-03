#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in mat4 aInstanceMatrix;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;
out float VariationFactor; 
out vec4 FragPosLightSpace;
out vec3 HeightTint; // Add a varying variable for the height tint color

uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

float random(vec3 position) {
    return fract(sin(dot(position, vec3(12.9898, 78.233, 45.543))) * 43758.5453);
}

void main()
{
    TexCoords = aTexCoords;    
    FragPos = vec3(aInstanceMatrix * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(aInstanceMatrix))) * aNormal; 
    
    vec3 position = aInstanceMatrix[3].xyz; // Extract position vector from the instance matrix
    VariationFactor = random(position); // Use the position vector to generate variation

    FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);

    // Calculate the height tint color using VariationFactor
    float height = position.y;
    if (height < 100.0) {
        HeightTint = vec3(0.043, 0.043, 0.043); // Black
    } else if (height < 200.0) {
        HeightTint = vec3(0.482, 0.372, 0.121); // Brown
    } else {
        HeightTint = vec3(0.671, 0.659, 0.678); // Silver Grey
    }

    gl_Position = projection * view * aInstanceMatrix * vec4(aPos, 1.0);
}
