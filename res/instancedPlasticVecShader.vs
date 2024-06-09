#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aInstancePos; 

out vec3 Normal;
out vec3 FragPos;
out float VariationFactor; 
out vec4 FragPosLightSpace;
out vec3 HeightTint; // Add a varying variable for the height tint color

uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

float random(vec3 position) {
    return fract(sin(dot(position, vec3(12.9898, 78.233, 45.543))) * 43758.5453);
}

void main()
{
    // Calculate world position using instance position and vertex position
    vec3 worldPos = aPos + aInstancePos;
    FragPos = worldPos;
    
    // Normal is directly aNormal since no transformation is required
    Normal = aNormal;

    // Calculate the variation factor using instance position
    VariationFactor = random(aInstancePos);

    // Transform FragPos to light space
    FragPosLightSpace = lightSpaceMatrix * vec4(worldPos, 1.0);

    // Calculate the height tint color based on height
    float height = aInstancePos.y;
    if (height < 100.0) {
        HeightTint = vec3(0.0, 0.298, 0.796); // Blue
    } else if (height < 200.0) {
        HeightTint = vec3(0.650, 0.843, 0.0); // Green
    } else {
        HeightTint = vec3(0.698, 0.0, 0.078); // Red
    }

    // Calculate final position
    gl_Position = projection * view * vec4(worldPos, 1.0);
}
