#ifndef AETNA_MATERIAL_H
#define AETNA_MATERIAL_H

#include "framework/shader/ShaderProgram.h"

class Material {
public:
	Material();
	~Material();

	ShaderProgram const* getShaderProgram();
	void setShaderProgram(ShaderProgram* shdr_prog);

	void save(const char *path);
	void load(const char *path);

	void apply();
private:
	ShaderProgram* m_shader_program;

	bool default_shader_program_used;
};

#endif //AETNA_MATERIAL_H