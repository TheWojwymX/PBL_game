#version 430 core
layout(location = 0) in vec3 aPos; // Position attribute

uniform mat4 viewProjection; // Model-view-projection matrix

void main() {
  gl_Position = viewProjection * vec4(aPos, 1.0);
}