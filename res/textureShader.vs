#version 430 core

layout (location = 0) in vec2 aPos;        // The position variable has attribute position 0
layout (location = 1) in vec2 aTexCoord;   // The texture coordinate variable has attribute position 1

uniform mat4 model;  // Model matrix

out vec2 TexCoord;   // Output to the fragment shader

void main() {
    gl_Position = model * vec4(aPos, 0.0, 1.0);  // Apply model transformation
    TexCoord = aTexCoord;  // Pass the texture coordinates to the fragment shader
}
