#version 330 core
in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D gSpecular;

struct Material {
    vec4 s_albedo_color;
    vec4 s_specular_color;
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


uniform vec3 viewPos;

vec3 calc_light_directional(LightDirectional light, Material material, vec3 normal, vec3 view_direction) {
    vec3 light_direction = normalize(-light.direction);
    float diffuse_shading = max(dot(normal, light_direction), 0.0f);
    
    vec3 reflect_direction = reflect(-light_direction, normal);
    float specular_shading = pow(max(dot(view_direction, reflect_direction), 0.0f), 0.8f);

    vec3 ambient = light.ambient * material.s_albedo_color.xyz;
    vec3 diffuse = light.diffuse * diffuse_shading * material.s_albedo_color.xyz;
    vec3 specular = light.specular * specular_shading * material.s_specular_color.xyz;

    return ambient + diffuse + specular;
}

vec3 calc_light_point(LightPoint light, Material material, vec3 normal, vec3 frag_pos, vec3 view_direction) {
    vec3 light_dir = normalize(light.position - frag_pos);
    float diffuse_shading = max(dot(normal, light_dir), 0.0f);

    vec3 reflect_direction = reflect(-light_dir, normal);
    float specular_shading = pow(max(dot(view_direction, reflect_direction), 0.0f), 0.8f);

    float distance = length(light.position - frag_pos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * distance * distance);

    vec3 ambient = light.ambient * material.s_albedo_color.xyz;
    vec3 diffuse = light.diffuse * diffuse_shading * material.s_albedo_color.xyz;
    vec3 specular = light.specular * specular_shading * material.s_specular_color.xyz;

    return (ambient + diffuse + specular) * attenuation;
}

void main()
{             
    // retrieve data from gbuffer
    vec4 FragPos = texture(gPosition, TexCoords);
    vec4 s_normal_color = texture(gNormal, TexCoords);
    vec4 Albedo = texture(gAlbedo, TexCoords);
    vec4 Specular = texture(gSpecular, TexCoords);
    
    // then calculate lighting as usual
    vec3 view_dir = normalize(viewPos - FragPos.rgb);
    vec3 lighting = vec3(0.0f);
    Material material;
    material.s_albedo_color = Albedo;
    material.s_specular_color = Specular;

    for (int i = 0; i < n_lights_directional; ++i) {
        lighting += calc_light_directional(lights_directional[i], material, s_normal_color.rgb, view_dir);
    }
    for (int i = 0; i < n_lights_point; ++i) {
        lighting += calc_light_point(lights_point[i], material, s_normal_color.rgb, FragPos.rgb, view_dir);
    }

	FragColor = vec4(lighting, 1.0);
}