#version 330 core

in vec3 fNormals;
in vec3 fPosition;

uniform vec4 object_color;
uniform vec3 view_position;

struct LightDirectional {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec4 light_color;
};

struct LightPoint {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec4 light_color;
    float constant;
    float linear;
    float quadratic;
};

struct LightSpot {
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec4 light_color;
    float cut_off;
};

#define MAX_LIGHTS 10
uniform int n_lights_directional;
uniform LightDirectional lights_directional[MAX_LIGHTS];
uniform int n_lights_point;
uniform LightPoint lights_point[MAX_LIGHTS];
uniform int n_lights_spot;
uniform LightSpot lights_spot[MAX_LIGHTS];

out vec4 fragcolor0;

vec3 calc_light_directional(LightDirectional light, vec3 normal, vec3 view_direction) {
    vec3 light_direction = normalize(-light.direction);
    float diffuse_shading = max(dot(normal, light_direction), 0.0f);
    
    vec3 reflect_direction = reflect(-light_direction, normal);
    //float specular_shading = pow(max(view_direction, reflect_direction), 0.0f), material.shininess;

    vec3 ambient = light.ambient * object_color.xyz;
    vec3 diffuse = light.diffuse * diffuse_shading * object_color.xyz;
    vec3 specular = vec3(0.0f, 0.0f, 0.0f);//light.specular * specular_shading * object_color.xyz;

    return ambient + diffuse + specular;
}

vec3 calc_light_point(LightPoint light, vec3 normal, vec3 frag_pos, vec3 view_dir) {
    vec3 light_dir = normalize(light.position - frag_pos);

    float diffuse_shading = max(dot(normal, light_dir), 0.0f);

    vec3 reflect_direction = reflect(-light_dir, normal);
    float specular_shading = 0.1f;//pow(max(dot(view_dir, reflect_direction), 0.0f), material.shininess);

    float distance = length(light.position - fPosition);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * distance * distance);

    vec3 ambient = light.ambient * object_color.xyz;
    vec3 diffuse = light.diffuse * diffuse_shading * object_color.xyz;
    vec3 specular = light.specular * specular_shading * object_color.xyz;

    return (ambient + diffuse + specular) * attenuation;
}

void main() {
    vec3 norm = normalize(fNormals);
    vec3 view_dir = normalize(view_position - fPosition);

    vec3 result = vec3(0.0f);

    for (int i = 0; i < n_lights_directional; ++i) {
        result += calc_light_directional(lights_directional[i], norm, view_dir);
    }
    for (int i = 0; i < n_lights_point; ++i) {
        result += calc_light_point(lights_point[i], norm, fPosition, view_dir);
    }
    //for (int i = 0; i < n_lights_spot; ++i) {
    //    result += calc_light_spot(lights_spot[i], norm, fPosition, view_dir);
    //}

    fragcolor0 = vec4(result, 1.0f);
}