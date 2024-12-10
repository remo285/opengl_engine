#version 410 core

out vec4 color;
in vec3 customColors;

void main() {
    color = vec4(customColors, 1.0f);
}