#version 330 core
in vec3 aPos;
in vec3 aNormal;
in vec2 aTexCoords;

out vec3 FragPos;
out vec2 TexCoords;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = (view * model * vec4(aPos, 1.0f)).xyz; 
    TexCoords = aTexCoords;
     
    Normal = mat3(transpose(inverse(view * model))) * aNormal;
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
}