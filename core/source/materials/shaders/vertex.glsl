#version 330 core

in vec3 pos;
in vec3 normals;
in vec2 texcoords;
in vec3 tangent;
in vec3 bitangent;  

out vec3 fNormals;
out vec3 fPosition;
out vec2 fTexCoord;
out mat3 fTBN;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(pos, 1.0f);
    fPosition = vec3(view * model * vec4(pos, 1.0f));
    fNormals = mat3(transpose(inverse(view * model))) * normals;
    fTexCoord = texcoords;

	vec3 T = normalize(vec3(model * vec4(tangent,   0.0)));
	vec3 B = normalize(vec3(model * vec4(bitangent, 0.0)));
	vec3 N = normalize(vec3(model * vec4(normals,    0.0)));
	mat3 TBN = mat3(T, B, N);
}