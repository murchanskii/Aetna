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

	void use();
private:
	ShaderProgram* m_shader_program;

	bool is_default;
};

#endif //AETNA_MATERIAL_H