#version 430 core
out vec4 FragColor;

// Define constants for lighting calculations
#define AMBIENT_STRENGTH 1.0

// Define structures for different types of lights
struct DirLight {
    vec3 direction;
    vec3 color;
    int isActive;
};

// Input from vertex shader
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in float VariationFactor; // Variation factor from the vertex shader
in float ClipDistance;

// Uniforms for view and lighting properties
uniform sampler2D texture_diffuse1;
uniform vec3 viewPos;
uniform DirLight dirLight;

// Function prototypes for lighting calculations
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);

void main()
{

    if (ClipDistance < 0) discard;
    // Normalize normal and view direction vectors
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // Calculate the directional light contribution (as before)
    vec3 dirLightColor = CalcDirLight(dirLight, norm, viewDir);

    // Modify the final color with a scaled color variation
    vec3 finalColor = dirLightColor;// Adjust the scale factor (0.1) as needed

    // Combine the final color with the texture color
    FragColor = vec4(finalColor, 0.85);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    if (light.isActive == 0)
        return vec3(0.0);
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // combine results
    vec3 ambient = light.color * AMBIENT_STRENGTH;
    vec3 diffuse = light.color * diff;
    return (ambient);
}