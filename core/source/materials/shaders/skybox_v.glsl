#version 330

in vec4 position;

uniform mat4 u_camera_projection; 
uniform mat4 u_camera_view;

smooth out vec3 f_eye_direction;

void main() {
    mat4 inv_projection = inverse(u_camera_projection);
    mat3 view_no_translate = transpose(mat3(u_camera_view));
    vec4 pos = position;
    vec3 unprojected = (inv_projection * pos).xyz;
    f_eye_direction = view_no_translate * unprojected;
    gl_Position = pos.xyww; 
}