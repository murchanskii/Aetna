#version 330

in vec2 ftexcoords; 

uniform sampler2D u_texture;

out vec4 fragmentColor; 

void main() {
    fragmentColor = texture(u_texture, ftexcoords); 
}