#ifndef AETNA_MATERIAL_H
#define AETNA_MATERIAL_H

#include "framework/shader/ShaderProgram.h"
#include "framework/Texture.h"

class Material {
public:
	Material();
	~Material();

	ShaderProgram *getShaderProgram();
	void setShaderProgram(ShaderProgram* shdr_prog);
	
	void save(const char *path);
	int load(const char *path);

	void setName(std::string name);
	std::string getName();

	Texture *getTextureAlbedo();

	void apply();
private:

	ShaderProgram* m_shader_program;
	std::vector<Texture*> m_vec_textures;
	bool default_shader_program_used;

	std::string m_name;
};

#endif //AETNA_MATERIAL_H