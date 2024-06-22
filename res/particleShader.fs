#version 430 core

in vec2 TexCoords;
in vec3 HeightTint;
out vec4 color;

uniform sampler2D sprite;
uniform vec3 dirColor;  // Directional light color
uniform vec4 pColor;
uniform bool showTexture;
uniform bool affectDirLight;
uniform bool plastic1;
uniform bool plastic2;

void main() {
    vec4 particleColor = pColor;

    if(plastic1 || plastic2){
    particleColor = vec4(HeightTint, 1.0);
    }

    vec4 texColor = showTexture ? texture(sprite, TexCoords) : vec4(1.0);
    vec3 dirLight = affectDirLight ? dirColor : vec3(0.8);
    color = particleColor * texColor * vec4(dirLight, 1.0);
}