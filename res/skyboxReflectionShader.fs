#version 430 core
out vec4 FragColor;
#define AMBIENT_STRENGHT 0.1
#define SPECULAR_STRENGHT 0.5
#define SHININESS 32

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform vec3 viewPos;
uniform samplerCube skybox;


void main()
{
    vec3 I = normalize(FragPos - viewPos);
    vec3 R = reflect(I, normalize(Normal));
    FragColor = vec4(texture(skybox, R).rgb, 1.0);
}