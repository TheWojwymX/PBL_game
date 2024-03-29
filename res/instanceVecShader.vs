#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in mat4 aInstanceMatrix;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;
out float VariationFactor; // Add a varying variable for the variation factor

uniform mat4 view;
uniform mat4 projection;

float random(vec3 position) {
    return fract(sin(dot(position, vec3(12.9898, 78.233, 45.543))) * 43758.5453);
}

void main()
{
    TexCoords = aTexCoords;    
    FragPos = vec3(aInstanceMatrix * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(aInstanceMatrix))) * aNormal; 
    
    // Calculate the variation factor
    vec3 position = aInstanceMatrix[3].xyz; // Extract position vector from the instance matrix
    VariationFactor = random(position); // Use the position vector to generate variation

    gl_Position = projection * view * aInstanceMatrix * vec4(aPos, 1.0);
}
