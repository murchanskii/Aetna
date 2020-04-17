#version 330

in vec3 pos;
in vec2 texcoords;

out vec2 ftexcoords;

void main() {
    ftexcoords = texcoords;
    gl_Position = vec4(pos, 1.0f); 
} 