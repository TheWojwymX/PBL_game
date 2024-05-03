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
uniform vec3 initialCloudPosition; // Add a uniform for cloud position
uniform float time;
uniform float cloudSpeed;

float random(vec3 position) {
    return fract(sin(dot(position, vec3(12.9898, 78.233, 45.543))) * 43758.5453);
}

void main()
{
    TexCoords = aTexCoords;

    // Adjust the entire instance matrix to prevent stretching due to boundary wrapping
    mat4 wrappedInstanceMatrix = aInstanceMatrix;

    // Boundary setup
    float boundary = 1400.0f;
    float offset = 2800.0f; // Total distance from -boundary to +boundary

    float moveX = mod(time * cloudSpeed, offset);
    vec3 cloudPosition = initialCloudPosition + vec3(moveX - boundary, 0.0, 0.0);

    // Wrap the cloud position at instance level
    wrappedInstanceMatrix[3].x += cloudPosition.x;

    if (wrappedInstanceMatrix[3].x > boundary) {
        wrappedInstanceMatrix[3].x -= offset;
    } else if (wrappedInstanceMatrix[3].x < -boundary) {
        wrappedInstanceMatrix[3].x += offset;
    }

    // Transform the position from model space to world space with wrapped instance matrix
    vec4 worldPosition = wrappedInstanceMatrix * vec4(aPos, 1.0);

    FragPos = vec3(worldPosition);
    Normal = mat3(transpose(inverse(wrappedInstanceMatrix))) * aNormal;

    // Calculate the variation factor
    VariationFactor = random(wrappedInstanceMatrix[3].xyz);

    gl_Position = projection * view * worldPosition;
}
