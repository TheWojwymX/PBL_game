#version 430 core

layout (location = 0) in vec4 vertex;  // <vec2 position, vec2 texCoords>
layout (location = 2) in vec3 instancePosition;  // Particle position
layout (location = 4) in float instanceScale;    // Particle scale

out vec2 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main() {
    // Get the camera's direction (typically the negative of the z-axis in the view matrix)
    vec3 cameraDirection = -normalize(vec3(view[0][2], view[1][2], view[2][2]));

    // Calculate camera right and up vectors using cross products
    vec3 cameraRight = normalize(cross(vec3(0.0, 1.0, 0.0), cameraDirection)); // World up vector crossed with camera direction
    vec3 cameraUp = normalize(cross(cameraDirection, cameraRight)); // Cross product to ensure correct orientation

    // Calculate the scaled billboard offset using the correct camera vectors
    vec3 billboardOffset = (cameraRight * (vertex.x * instanceScale)) + (cameraUp * (vertex.y * instanceScale));

    // Calculate the final position, using billboarding logic
    vec3 finalPosition = instancePosition + billboardOffset;

    // Pass the texture coordinates to the fragment shader
    TexCoords = vertex.zw;

    // Apply the transformation matrices
    gl_Position = projection * view * vec4(finalPosition, 1.0);
}