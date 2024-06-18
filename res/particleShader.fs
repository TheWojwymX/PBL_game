#version 430 core

in vec2 TexCoords;
out vec4 color;

uniform sampler2D sprite;
uniform vec3 dirColor;  // Directional light color
uniform vec4 pColor;
uniform bool showTexture;
uniform bool affectDirLight;

void main() {
    vec4 texColor = showTexture ? texture(sprite, TexCoords) : vec4(1.0);
    vec3 dirLight = affectDirLight ? dirColor : vec3(0.8);
    color = pColor * texColor * vec4(dirLight, 1.0);
}