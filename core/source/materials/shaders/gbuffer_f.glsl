#version 330 core
in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

out vec3 gPosition;
out vec3 gNormal;
out vec4 gAlbedo;
out vec4 gSpecular;

uniform vec4 albedo_color;

struct Material {
    // Textures
    sampler2D albedo_texture;
    //sampler2D normal_texture;
    sampler2D specular_texture;

    // Colors
    float shininess;
};

uniform Material material;

void main()
{
    vec4 s_albedo_color = texture(material.albedo_texture, TexCoords) * albedo_color;
    vec4 s_specular_color = texture(material.specular_texture, TexCoords);


    // store the fragment position vector in the first gbuffer texture
    gPosition = FragPos;
    // also store the per-fragment normals into the gbuffer
    gNormal = normalize(Normal);
    // and the diffuse per-fragment color
    gAlbedo = s_albedo_color;
    // store specular intensity
    gSpecular = s_specular_color;
}