#version 430 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 additionalColor;

void main()
{
    vec3 finalColor = additionalColor;
    
    // Set transparency based on the condition of FragPos.y
    float transparency = (FragPos.y > 299.6) ? 0.15 : 0.5;
    
    FragColor = vec4(finalColor, transparency);
}
