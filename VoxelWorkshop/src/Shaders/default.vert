#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

// Outputs color to fragment shader
out vec3 color;

uniform mat4 camMatrix;

void main() {
    gl_Position = camMatrix * vec4(aPos, 1.0f);
    color = aColor;
}