#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aInstancePos; // Changed from mat4 to vec3

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
    // Adjust the entire instance position to prevent stretching due to boundary wrapping
    vec3 wrappedInstancePos = aInstancePos; // Applying scaling factors

    // Boundary setup
    float boundary = 1400.0f;
    float offset = 2800.0f; // Total distance from -boundary to +boundary

    float moveX = mod(time * cloudSpeed, offset);
    vec3 cloudPosition = initialCloudPosition + vec3(moveX - boundary, 0.0, 0.0);

    // Wrap the cloud position at instance level
    wrappedInstancePos.x += cloudPosition.x;

    if (wrappedInstancePos.x > boundary) {
        wrappedInstancePos.x -= offset;
    } else if (wrappedInstancePos.x < -boundary) {
        wrappedInstancePos.x += offset;
    }

    // Transform the position from model space to world space with wrapped instance position
    vec4 worldPosition = vec4((aPos * vec3(70.0, 8.5, 70.0)) + wrappedInstancePos, 1.0);

    FragPos = vec3(worldPosition);
    Normal = aNormal;

    // Calculate the variation factor
    VariationFactor = random(wrappedInstancePos);

    gl_Position = projection * view * worldPosition;
}
