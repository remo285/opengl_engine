#version 410 core

out vec4 color;

in vec3 customColors;
in vec2 texCoords;

uniform sampler2D texture0;
uniform sampler2D texture1;

void main() {
    color = mix(texture(texture0, texCoords), texture(texture1, texCoords), 0.2);
}