#ifndef AETNA_MATERIAL_H
#define AETNA_MATERIAL_H

#include <framework/render/shader/ShaderProgram.h>
#include <Texture.h>

class Material {
public:
	Material();
	~Material();

	void apply();

	ShaderProgram *getShaderProgram();
	void setShaderProgram(ShaderProgram* shdr_prog);
	
	void save(const char *path);
	int load(const char *path);

	void setName(std::string name);
	std::string getName();

	enum class TextureType {
		ALBEDO = 0,
		NORMAL,
		SPECULAR,
		COUNT			// not a type
	};

	Texture *getTexture(TextureType type);
	const char* getTextureName(TextureType type);
	void setTexture(TextureType type, const char *path);
	void setTexture(TextureType type, Texture *texture);

private:

	ShaderProgram* m_shader_program;
	bool default_shader_program_used;

	struct TextureVariable {
		std::string name;
		Texture* texture;
	};

	std::string m_name;

	TextureVariable tex_albedo;
	TextureVariable tex_normal;
	TextureVariable tex_specular;
};

#endif //AETNA_MATERIAL_H