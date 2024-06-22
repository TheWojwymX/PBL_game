#version 430 core

layout (location = 0) in vec4 vertex;  // <vec2 position, vec2 texCoords>
layout (location = 2) in vec3 instancePosition;  // Particle position
layout (location = 4) in float instanceScale;    // Particle scale

out vec2 TexCoords;
out vec3 HeightTint;

uniform mat4 projection;
uniform mat4 view;

uniform bool plastic1;
uniform bool plastic2;

void main() {

    if (instanceScale > 2.0) {
            gl_Position = vec4(2.0, 2.0, 2.0, 1.0); // Move vertex out of the clip space
            return;
        }

    if(plastic2){
        float height = instancePosition.y;
        if (height < 100.0) {
            HeightTint = vec3(0.0, 0.298, 0.796); // Blue
        } else if (height < 200.0) {
            HeightTint = vec3(0.650, 0.843, 0.0); // Green
        } else {
            HeightTint = vec3(0.698, 0.0, 0.078); // Red
        }
    }

    if(plastic1){
        float height = instancePosition.y;
        if (height < 100.0) {
            HeightTint = vec3(0.282, 0.820, 0.800); // Turquoise
        } else if (height < 200.0) {
            HeightTint = vec3(0.000, 0.502, 0.502); // Teal
        } else {
            HeightTint = vec3(0.000, 0.000, 0.502); // Navy Blue
        }
    }

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