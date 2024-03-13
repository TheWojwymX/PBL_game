#version 430 core
layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>

out vec2 TexCoords;
out vec4 ParticleColor;

uniform mat4 projection;
uniform mat4 view;
uniform vec3 offset;
uniform vec4 color;

void main()
{
    float scale = 0.3f;
    TexCoords = vertex.zw;
    ParticleColor = color;

    vec3 cameraRight = normalize(vec3(view[0][0], view[1][0], view[2][0]));
    vec3 cameraUp = normalize(vec3(view[0][1], view[1][1], view[2][1]));

    // Calculate the final position using billboarding
    vec3 finalPosition = vec3((vertex.xy) * scale, 0.0) + offset;
    finalPosition = finalPosition + cameraRight * vertex.x + cameraUp * vertex.y;

    gl_Position = projection * view * vec4(finalPosition, 1.0);
}
