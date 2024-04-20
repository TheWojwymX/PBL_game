#version 430 core
out vec4 FragColor;

// Define constants for lighting calculations
#define AMBIENT_STRENGTH 0.1
#define SPECULAR_STRENGTH 0.5
#define SHININESS 32
#define NR_SPOT_LIGHTS 1

// Define structures for different types of lights
struct DirLight {
    vec3 direction;
    vec3 color;
    int isActive;
};

struct PointLight {
    vec3 position;
    float constant;
    float linear;
    float quadratic;
    vec3 color;
    int isActive;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
    float constant;
    float linear;
    float quadratic;
    vec3 color;
    bool isActive;
};

// Input from vertex shader
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in float VariationFactor; // Variation factor from the vertex shader

// Uniforms for view and lighting properties
uniform sampler2D texture_diffuse1;
uniform vec3 viewPos;
uniform DirLight dirLight;
uniform SpotLight spotLights[NR_SPOT_LIGHTS];

// Function prototypes for lighting calculations
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    // Normalize normal and view direction vectors
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 spotLightColor = vec3(0.0);

    // Calculate color variation based on the variation factor
    vec3 colorVariation = vec3(sin(VariationFactor), cos(VariationFactor), tan(VariationFactor));

    // Calculate the directional light contribution (as before)
    vec3 dirLightColor = CalcDirLight(dirLight, norm, viewDir);

    for(int i = 0; i < NR_SPOT_LIGHTS; i++)
    {
        if(spotLights[i].isActive)
        {
            spotLightColor += CalcSpotLight(spotLights[i], norm, FragPos, viewDir);
        }
    }

    // Modify the final color with a scaled color variation
    vec3 finalColor = (dirLightColor + spotLightColor) * (1.0 + 0.1 * colorVariation); // Adjust the scale factor (0.1) as needed

    // Combine the final color with the texture color
    FragColor = vec4(finalColor, 1.0) * texture(texture_diffuse1, TexCoords);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    if (light.isActive == 0)
        return vec3(0.0);
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), SHININESS);
    // combine results
    vec3 ambient = light.color * AMBIENT_STRENGTH;
    vec3 diffuse = light.color * diff;
    vec3 specular = light.color * spec * SPECULAR_STRENGTH;
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    if (light.isActive == 0)
        return vec3(0.0);
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), SHININESS);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // combine results
    vec3 ambient = light.color * AMBIENT_STRENGTH;
    vec3 diffuse = light.color * diff;
    vec3 specular = light.color * spec * SPECULAR_STRENGTH;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    if (light.isActive == false)
        return vec3(0.0);
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), SHININESS);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.color * AMBIENT_STRENGTH;
    vec3 diffuse = light.color * diff;
    vec3 specular = light.color * spec * SPECULAR_STRENGTH;
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}