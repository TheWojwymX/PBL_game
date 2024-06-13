#version 430 core

in vec2 TexCoord; // Input texture coordinates from the vertex shader

out vec4 FragColor; // Output color to the framebuffer

uniform sampler2D texture1; // The texture sampler

void main() {
    FragColor = texture(texture1, TexCoord); // Sample the texture at the given coordinates
}
