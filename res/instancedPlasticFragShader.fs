#version 430 core
out vec4 FragColor;

// Define constants for lighting calculations
#define AMBIENT_STRENGTH 0.25
#define SPECULAR_STRENGTH 0.6
#define SHININESS 50
#define NR_SPOT_LIGHTS 1
#define NR_POINT_LIGHTS 50

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
    bool isActive;
    bool isShot;
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
in float VariationFactor; // Variation factor from the vertex shader
in vec4 FragPosLightSpace;
in vec3 HeightTint; // Add height tint color input

// Uniforms for view and lighting properties
uniform sampler2D shadowMap;
uniform vec3 viewPos;
uniform vec3 lightPos;
uniform DirLight dirLight;
uniform SpotLight spotLights[NR_SPOT_LIGHTS];
uniform PointLight pointLights[NR_POINT_LIGHTS];

// Function prototypes for lighting calculations
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
float ShadowCalculation(vec4 fragPosLightSpace, float spotlightIntensity, float pointLightIntensity);
float CalcSpotLightIntensity(SpotLight spotLight, vec3 fragPos);
float CalcPointLightIntensity(PointLight pointLight, vec3 fragPos);

void main()
{
    // Normalize normal and view direction vectors
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 spotLightColor = vec3(0.0);
    vec3 pointLightColor = vec3(0.0);
    float totalSpotlightIntensity = 0.0f;
    float totalPointlightIntensity = 0.0f;

    // Calculate the directional light contribution with height and position-based attenuation
    vec3 dirLightColor = CalcDirLight(dirLight, norm, viewDir);

    for(int i = 0; i < NR_SPOT_LIGHTS; i++)
    {
        if(spotLights[i].isActive)
        {
            spotLightColor += CalcSpotLight(spotLights[i], norm, FragPos, viewDir);
            totalSpotlightIntensity += CalcSpotLightIntensity(spotLights[i], FragPos);
        }
    }

    // Calculate point light contributions
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
    {
        if(pointLights[i].isActive && !pointLights[i].isShot)
        {
            float distance = length(pointLights[i].position - FragPos);
            float maxDistance = 60.0;

            if (distance < maxDistance)
            {
                pointLightColor += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
                totalPointlightIntensity += CalcPointLightIntensity(pointLights[i], FragPos);
            }
        }
    }

    vec3 clamped = clamp(pointLightColor, vec3(0.0,0.0,0.0),  vec3(1.0,1.0,1.0));

    float shadow = ShadowCalculation(FragPosLightSpace, totalSpotlightIntensity, totalPointlightIntensity);

    // Apply height-based tint directly
    vec3 heightTint = HeightTint;

    // Modify the final color with a smaller scaled variation factor for brightness adjustment
    float brightnessFactor = 1.0 + (VariationFactor - 0.5) * 0.05; // Reduce the effect of brightness variation
    vec3 finalColor = (spotLightColor + clamped) * shadow * heightTint * brightnessFactor;

    // Combine the final color with the texture color
    FragColor = vec4(finalColor, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    if (light.isActive == 0)
        return vec3(0.0);
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.8);
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
    if (!light.isActive) return vec3(0.0);
    float distance = length(light.position - fragPos);
    float attenuation = 6.5 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    vec3 ambient = vec3(1.0,1.0,1.0) * attenuation * 10;

// Diffuse component
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = vec3(1.0,1.0,1.0) * diff * attenuation;

    // Specular component
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), SHININESS); // 32 is the shininess exponent, adjust as needed
        vec3 specular = vec3(1.0,1.0,1.0) * SPECULAR_STRENGTH * spec * attenuation;

    return ambient + specular;
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
    vec3 specular = light.color * SPECULAR_STRENGTH;
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (specular + ambient + diffuse); //+specular
}

float ShadowCalculation(vec4 fragPosLightSpace, float spotlightIntensity, float pointLightIntensity)
{
    float shadow = 1.0f;
    shadow = mix(shadow, 1.0, clamp(spotlightIntensity + pointLightIntensity + 0.14, 0.0, 1.0));
    return shadow - 0.2f;
}

float CalcSpotLightIntensity(SpotLight spotLight, vec3 fragPos)
{
    // Determine the direction of the spotlight
    vec3 lightDir = normalize(spotLight.position - fragPos);

    // Calculate the distance between the spotlight and the fragment
    float distance = length(spotLight.position - fragPos);

    // Calculate attenuation based on distance
    float attenuation = 2.0 / (spotLight.constant + spotLight.linear * distance + spotLight.quadratic * (distance * distance));

    // Calculate the spotlight intensity based on the angle within the cone
    float theta = dot(lightDir, normalize(-spotLight.direction));
    float epsilon = spotLight.cutOff - spotLight.outerCutOff;
    float intensity = clamp((theta - spotLight.outerCutOff) / epsilon, 0.13, 1.0); //Flashlight effect on shadow

    // Final intensity considering attenuation and angle
    return attenuation * intensity;
}

float CalcPointLightIntensity(PointLight pointLight, vec3 fragPos)
{
    // Calculate the distance between the point light and the fragment
    float distance = length(pointLight.position - fragPos);

    // Calculate attenuation based on distance
    float attenuation = 2.0 / (pointLight.constant + pointLight.linear * distance + pointLight.quadratic * (distance * distance));

    // The intensity of a point light does not depend on angle, only on attenuation
    return attenuation;
}
