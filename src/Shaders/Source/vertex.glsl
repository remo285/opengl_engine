#version 410 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 textureCoordinates;

out vec3 customColors;
out vec2 texCoords;

uniform float hOffset;

void main() {
    gl_Position = vec4(position.x + hOffset, position.yzw);
    customColors = color;
    texCoords = textureCoordinates;
}