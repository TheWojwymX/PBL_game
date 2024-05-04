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
in vec4 FragPosLightSpace;

// Uniforms for view and lighting properties
uniform sampler2D texture_diffuse1;
uniform sampler2D shadowMap;
uniform vec3 viewPos;
uniform vec3 lightPos;
uniform DirLight dirLight;
uniform SpotLight spotLights[NR_SPOT_LIGHTS];

// Function prototypes for lighting calculations
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
float ShadowCalculation(vec4 fragPosLightSpace, float spotlightIntensity);
float CalcSpotLightIntensity(SpotLight spotLight, vec3 fragPos);

void main()
{
    // Normalize normal and view direction vectors
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 spotLightColor = vec3(0.0);
    float totalSpotlightIntensity = 0.0f;

    // Calculate color variation based on the variation factor
    vec3 colorVariation = vec3(sin(VariationFactor), cos(VariationFactor), tan(VariationFactor));

    // Calculate the directional light contribution (as before)
    vec3 dirLightColor = CalcDirLight(dirLight, norm, viewDir);

    for(int i = 0; i < NR_SPOT_LIGHTS; i++)
    {
        if(spotLights[i].isActive)
        {
            spotLightColor += CalcSpotLight(spotLights[i], norm, FragPos, viewDir);
            totalSpotlightIntensity += CalcSpotLightIntensity(spotLights[i], FragPos);
        }
    }

    float shadow = ShadowCalculation(FragPosLightSpace, totalSpotlightIntensity);

    // Modify the final color with a scaled color variation
    vec3 finalColor = (dirLightColor + spotLightColor) * shadow * (1.0 + 0.1 * colorVariation); // Adjust the scale factor (0.1) as needed

    // Combine the final color with the texture color
    FragColor = vec4(finalColor, 1.0) * texture(texture_diffuse1, TexCoords);
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
    return (ambient + diffuse); //+specular
}

float ShadowCalculation(vec4 fragPosLightSpace, float spotlightIntensity)
{
      // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
     //float bias =  0.005;
     vec3 normal = normalize(Normal);
     vec3 lightDir = normalize(lightPos - FragPos);
     float bias = max(0.008 * (1.0 - dot(normal, lightDir)), 0.005);
     //float shadow = (currentDepth - bias) > closestDepth  ? 0.4 : 1.0;

     float shadow = 0.0;
         vec2 texelSize = 0.3 / textureSize(shadowMap, 0);
         for(int x = -1; x <= 1; ++x)
         {
             for(int y = -1; y <= 1; ++y)
             {
                 float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
                 shadow += currentDepth - bias > pcfDepth  ? 0.0 : 1.0;
             }
         }
         shadow /= 9.0;

     shadow = mix(shadow, 1.0, clamp(spotlightIntensity + 0.3,0.0,1.0)); // Blend shadow with spotlight intensity

    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
        if(projCoords.z > 1.0)
            shadow = 0.0;

    return shadow - 0.2;
}

float CalcSpotLightIntensity(SpotLight spotLight, vec3 fragPos)
{
    // Determine the direction of the spotlight
    vec3 lightDir = normalize(spotLight.position - fragPos);

    // Calculate the distance between the spotlight and the fragment
    float distance = length(spotLight.position - fragPos);

    // Calculate attenuation based on distance
    float attenuation = 1.0 / (spotLight.constant + spotLight.linear * distance + spotLight.quadratic * (distance * distance));

    // Calculate the spotlight intensity based on the angle within the cone
    float theta = dot(lightDir, normalize(-spotLight.direction));
    float epsilon = spotLight.cutOff - spotLight.outerCutOff;
    float intensity = clamp((theta - spotLight.outerCutOff) / epsilon, 0.0, 1.0);

    // Final intensity considering attenuation and angle
    return attenuation * intensity;
}