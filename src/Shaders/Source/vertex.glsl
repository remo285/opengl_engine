#version 410 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 color;

out vec3 customColors;

void main() {
    gl_Position = position;
    customColors = color;
}