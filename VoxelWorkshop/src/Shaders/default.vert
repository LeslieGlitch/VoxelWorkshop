#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

// Outputs color to fragment shader
out vec3 color;

// Controls scale of the vertices
uniform float scale;

// Import matrices for 3D rendering
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main() {
    gl_Position = proj *  view * model * vec4(aPos, 1.0f);
    color = aColor;
}