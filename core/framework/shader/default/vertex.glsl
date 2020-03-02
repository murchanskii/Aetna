#version 330 core

in vec3 pos;
in vec3 normals;

out vec3 fNormals;
out vec3 fPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(pos, 1.0f);
    fPosition = vec3(view * model * vec4(pos, 1.0f));
    fNormals = mat3(transpose(inverse(view * model))) * normals;
}