#include "Material.h"
#include "framework/shader/OpenGLShaderProgram.h"

Material::Material() {
	is_default = true;
	m_shader_program = new OpenGLShaderProgram();
	m_shader_program->setVertexShader("../core/framework/shader/default/vertex.glsl");
	m_shader_program->setFragmentShader("../core/framework/shader/default/fragment.glsl");
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
