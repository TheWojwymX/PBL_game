#version 430 core

out vec4 color;

uniform sampler2D sprite;
uniform vec3 dirColor;  // Directional light color
uniform vec4 pColor;

void main() {
    color = pColor * vec4(dirColor, 1.0);
}