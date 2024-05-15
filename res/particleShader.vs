#version 430 core
layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>

out vec2 TexCoords;
out vec4 ParticleColor;

uniform mat4 projection;
uniform mat4 view;
uniform vec3 offset;
uniform vec4 color;
uniform float scale;

void main()
{
    // Particle texture coordinates and color
    TexCoords = vertex.zw;
    ParticleColor = color;

    // Get the camera's direction (typically the negative of the z-axis in the view matrix)
    vec3 cameraDirection = -normalize(vec3(view[0][2], view[1][2], view[2][2]));

    // Calculate camera right and up vectors using cross products
    vec3 cameraRight = normalize(cross(vec3(0.0, 1.0, 0.0), cameraDirection)); // World up vector crossed with camera direction
    vec3 cameraUp = normalize(cross(cameraDirection, cameraRight)); // Cross product to ensure correct orientation

    // Calculate the base position
    vec3 basePosition = offset;

    // Calculate the scaled billboard offset using the correct camera vectors
    vec3 billboardOffset = (cameraRight * (vertex.x * scale)) + (cameraUp * (vertex.y * scale));

    // Calculate the final position, using billboarding logic
    vec3 finalPosition = basePosition + billboardOffset;

    // Apply the transformation matrices
    gl_Position = projection * view * vec4(finalPosition, 1.0);
}