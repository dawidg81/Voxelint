#version 330 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inColor; // normalized automatically

out vec3 fragColor;

uniform mat4 uViewProj;

void main() {
    fragColor = inColor;
    gl_Position = uViewProj * vec4(inPos, 1.0);
}
