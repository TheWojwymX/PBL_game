#version 430 core

in vec2 TexCoords;
out vec4 color;

uniform sampler2D sprite;
uniform vec3 dirColor;  // Directional light color
uniform vec4 pColor;

void main() {
    color = pColor * texture(sprite, TexCoords) * vec4(dirColor, 1.0);
}