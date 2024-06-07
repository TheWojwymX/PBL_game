#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform vec3 additionalColor;
uniform bool useColor;

void main()
{
    vec4 texColor = texture(texture1, TexCoord);
    if (useColor) {
        FragColor = texColor * vec4(additionalColor, 1.0);
    } else {
        FragColor = texColor;
    }
}
