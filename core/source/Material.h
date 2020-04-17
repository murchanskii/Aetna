#ifndef AETNA_MATERIAL_H
#define AETNA_MATERIAL_H

#include <framework/render/shader/ShaderProgram.h>

class Material {
public:
	Material();
	~Material();

	void apply();
	void use();
	
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

	void addVertexShaderSource(const char* path);
	void addVertexShaderContents(const char* code);

	void addFragmentShaderSource(const char* path);
	void addFragmentShaderContents(const char* code);

private:
	ShaderProgram* m_shader_program = nullptr;
	std::vector<Shader*> m_vec_vertex_shaders;
	std::vector<Shader*> m_vec_fragment_shaders;
	std::string m_name;
	std::vector<std::string> m_vec_vertex_shader_paths;
	std::vector<std::string> m_vec_fragment_shader_paths;

	void initialize_shader_program_and_shaders();
	void remove_shader_program_and_shaders();
};

#endif //AETNA_MATERIAL_H