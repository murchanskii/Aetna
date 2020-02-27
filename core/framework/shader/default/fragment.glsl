#version 330 core

in vec3 fNormals;
in vec3 fPosition;

uniform vec4 new_color;

out vec4 fragcolor0;

void main() {
    fragcolor0 = new_color;//vec4(1.0f, 0.0f, 0.0f, 1.0f);
}