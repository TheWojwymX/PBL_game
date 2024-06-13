#version 430 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;

void main()
{
    vec3 color = texture(skybox, TexCoords).rgb;

        // Calculate the direction from the view position to the light position
        vec3 lightDir = normalize(lightPos - viewPos);

        // Calculate the direction from the view position to the texel in the skybox
        vec3 viewDir = normalize(TexCoords);

        // Calculate the dot product of viewDir and lightDir
        float dotProduct = dot(viewDir, lightDir);

        // Check if the texture color is red
        bool isRedTexture = (color.r > 0.5 && color.g < 0.7 && color.b < 0.5);

        // Simulate lighting effect by using a simple dot product
        float diff = isRedTexture ? max(dotProduct, 1.0) : max(dotProduct, 0.6);

        // Calculate weighting factor based on the blue component
        float blueWeight = color.b;
        float redWeight = color.r;

        // Adjust the color based on the diffuse lighting term and blue weight
        vec3 finalColor = color * (0.1 + 0.7 * diff * lightColor * (1.0 + 1.3));

        FragColor = vec4(finalColor, 1.0);
    }