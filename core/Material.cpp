#include "Material.h"
#include "framework/shader/OpenGLShaderProgram.h"
#include "framework/Engine.h"

Material::Material() {
	is_default = true;
	m_shader_program = new OpenGLShaderProgram();

	std::string path_to_core;
	std::string path_to_exe = Engine::get()->getExecutablePath();
	int proj_name_index =  path_to_exe.find("Aetna");
	if (proj_name_index >= 0) {
		path_to_core = path_to_exe.substr(0, proj_name_index + strlen("Aetna") + 1) + "core\\";
	}

	m_shader_program->setVertexShader((path_to_core + std::string("framework/shader/default/vertex.glsl")).c_str());
	m_shader_program->setFragmentShader((path_to_core + std::string("framework/shader/default/fragment.glsl")).c_str());
}

Material::~Material() {
	if (is_default) {
		delete m_shader_program;
	}
}

ShaderProgram const* Material::getShaderProgram() {
	return m_shader_program;
}

void Material::setShaderProgram(ShaderProgram* shdr_prog) {
	is_default = false;
	m_shader_program = shdr_prog;
}

void Material::save(const char* path) {
}

void Material::load(const char* path) {
}

void Material::apply() {
	m_shader_program->use();
}
