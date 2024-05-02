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
uniform vec3 cloudPosition; // Add a uniform for cloud position

float random(vec3 position) {
    return fract(sin(dot(position, vec3(12.9898, 78.233, 45.543))) * 43758.5453);
}

void main()
{
    TexCoords = aTexCoords;

    // Transform the position from model space to world space
    vec4 worldPosition = aInstanceMatrix * vec4(aPos, 1.0);
    worldPosition.xyz += cloudPosition;

    // Check if the cloud position is beyond the x boundary (200.0 units here)
    float boundary = 500.0f;
    float offset = 1000.0f; // Total distance from -boundary to +boundary

    // Calculate the wrapped x position
    if (worldPosition.x > boundary) {
        worldPosition.x = -boundary + mod(worldPosition.x - boundary, offset);
    } else if (worldPosition.x < -boundary) {
        worldPosition.x = boundary - mod(-boundary - worldPosition.x, offset);
    }

    FragPos = vec3(worldPosition);
    Normal = mat3(transpose(inverse(aInstanceMatrix))) * aNormal;

    // Calculate the variation factor
    vec3 position = aInstanceMatrix[3].xyz + cloudPosition; // Updated to include cloudPosition
    VariationFactor = random(position); // Use the position vector to generate variation

    gl_Position = projection * view * worldPosition;
}