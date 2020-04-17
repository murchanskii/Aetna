#version 330

smooth in vec3 f_eye_direction; 

uniform samplerCube u_texture;

out vec4 fragmentColor; 

void main() {
    fragmentColor = texture(u_texture, f_eye_direction); 
}