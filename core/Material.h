#ifndef AETNA_MATERIAL_H
#define AETNA_MATERIAL_H

#include "framework/shader/ShaderProgram.h"

#include <pugixml.hpp>

class Material {
public:
	static Material* create(const char *name);

	Material();
	~Material();

	ShaderProgram *getShaderProgram();
	void setShaderProgram(ShaderProgram* shdr_prog);
	
	void save(const char *path);
	int load(const char *path);

	void setName(std::string name);
	std::string getName();
	std::string getPath();

	void apply();
private:
	ShaderProgram* m_shader_program;
	bool default_shader_program_used;

	std::string m_name;
	std::string m_path;

	std::string getStringVariable(Variable *variable);
	void parseXmlVariable(pugi::xml_node variable_node);

	int parseXmlVariableInt(const char* str_var);
	float parseXmlVariableFloat(const char* str_var);
	glm::vec3 parseXmlVariableVec3(const char* str_var);
	glm::vec4 parseXmlVariableVec4(const char* str_var);
};

#endif //AETNA_MATERIAL_H