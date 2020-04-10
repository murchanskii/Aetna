#ifndef AETNA_MATERIAL_H
#define AETNA_MATERIAL_H

#include <framework/render/shader/ShaderProgram.h>

class Material {
public:
	Material();
	~Material();

	void apply();
	
	void save(const char *path);
	int load(const char *path);

	void setName(std::string name);
	const char *getName();

	void setVariable(const char *name, Variable *variable);
	Variable *getVariable(const char* name);
	const char *getVariableName(int index);
	int getNumVariables();

	Texture getTexture(const char* name);
	const char *getTextureName(int index);
	void setTexture(const char* name, Texture texture); 
	int getNumTextures();

	void setVertexShaderSource(const char* path);
	void setVertexShaderContents(const char* code);

	void setFragmentShaderSource(const char* path);
	void setFragmentShaderContents(const char* code);

private:
	ShaderProgram* m_shader_program = nullptr;
	Shader* m_vertex_shader = nullptr;
	Shader* m_fragment_shader = nullptr;
	std::string m_name;
	std::string m_vertex_shader_path;
	std::string m_fragment_shader_path;

	bool program_ready_to_link();
	void initialize_shader_program_and_shaders();
	void remove_shader_program_and_shaders();
};

#endif //AETNA_MATERIAL_H