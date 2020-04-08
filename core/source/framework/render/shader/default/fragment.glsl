#version 330 core

in vec3 fNormals;
in vec3 fPosition;
in vec2 fTexCoord;
in mat3 fTBN;

uniform vec3 view_position;

uniform vec4 albedo_color;

struct Material {
    // Textures
    sampler2D albedo_texture;
    sampler2D normal_texture;
    sampler2D specular_texture;

    // Colors
    float shininess;
};

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

uniform Material material;

out vec4 fragcolor0;

vec3 calc_light_directional(LightDirectional light, vec3 normal, vec3 view_direction) {
    vec4 s_albedo_color = texture(material.albedo_texture, fTexCoord) * albedo_color;
    vec4 s_specular_color = texture(material.specular_texture, fTexCoord);

    vec3 light_direction = normalize(-light.direction);
    float diffuse_shading = max(dot(normal, light_direction), 0.0f);
    
    vec3 reflect_direction = reflect(-light_direction, normal);
    float specular_shading = pow(max(dot(view_direction, reflect_direction), 0.0f), 0.8f);

    vec3 ambient = light.ambient * s_albedo_color.xyz;
    vec3 diffuse = light.diffuse * diffuse_shading * s_albedo_color.xyz;
    vec3 specular = light.specular * specular_shading * s_specular_color.xyz;

    return ambient + diffuse + specular;
}

vec3 calc_light_point(LightPoint light, vec3 normal, vec3 frag_pos, vec3 view_direction) {
    vec4 s_albedo_color = texture(material.albedo_texture, fTexCoord) * albedo_color;
    vec4 s_specular_color = texture(material.specular_texture, fTexCoord);

    vec3 light_dir = normalize(light.position - frag_pos);
    float diffuse_shading = max(dot(normal, light_dir), 0.0f);

    vec3 reflect_direction = reflect(-light_dir, normal);
    float specular_shading = pow(max(dot(view_direction, reflect_direction), 0.0f), 0.8f);

    float distance = length(light.position - fPosition);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * distance * distance);

    vec3 ambient = light.ambient * s_albedo_color.xyz;
    vec3 diffuse = light.diffuse * diffuse_shading * s_albedo_color.xyz;
    vec3 specular = light.specular * specular_shading * s_specular_color.xyz;

    return (ambient + diffuse + specular) * attenuation;
}

void main() {
    vec3 s_normal_color = normalize(fNormals);
    //vec4 s_normal_color = texture(material.normal_texture, fTexCoord);
    //s_normal_color = normalize(s_normal_color * 2.0 - 1.0);

    vec3 view_dir = normalize(view_position - fPosition);
    vec3 result = vec3(0.0f);

    for (int i = 0; i < n_lights_directional; ++i) {
        result += calc_light_directional(lights_directional[i], s_normal_color, view_dir);
    }
    for (int i = 0; i < n_lights_point; ++i) {
        result += calc_light_point(lights_point[i], s_normal_color, fPosition, view_dir);
    }
    //for (int i = 0; i < n_lights_spot; ++i) {
    //    result += calc_light_spot(lights_spot[i], s_normal_color, fPosition, view_dir);
    //}

    fragcolor0 = vec4(result, 1.0f);
}