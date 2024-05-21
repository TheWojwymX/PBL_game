#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;
out vec4 FragPosLightSpace;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;
uniform vec3 offsets[8]; // Tablica nowych pozycji dla każdego wierzchołka

void main()
{
    TexCoords = aTexCoords;

    // Zakładamy, że wierzchołki są indeksowane od 0 do 7
    int vertexID = gl_VertexID % 8;

    // Użyj nowej pozycji wierzchołka z tablicy offsets zamiast aPos
    vec3 newPos = offsets[vertexID];

    FragPos = vec3(model * vec4(newPos, 1.0)); // Podmiana konkretnego wierzchołka
    Normal = mat3(transpose(inverse(model))) * aNormal;
    FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
    gl_Position = projection * view * model * vec4(newPos, 1.0); // Podmiana konkretnego wierzchołka
}
