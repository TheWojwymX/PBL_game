#version 430 core
in vec2 TexCoords;
in vec4 ParticleColor;
out vec4 color;

uniform sampler2D sprite;
uniform vec3 dirColor;
void main()
{
    color = texture(sprite, TexCoords) * ParticleColor * vec4(dirColor, 1.0);
}  