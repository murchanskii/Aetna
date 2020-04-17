#include "Material.h"
#include <framework/Engine.h>
#include <framework/render/shader/OpenGLShaderProgram.h>
#include <framework/Materials.h>
#include <Utils.h>

#include <pugixml.hpp>

#include <iostream>

Material::Material() {
	initialize_shader_program_and_shaders();
}

Material::~Material() {
	remove_shader_program_and_shaders();
}

void Material::save(const char* path) {
	OpenGLShaderProgram* ogl_shader_program = dynamic_cast<OpenGLShaderProgram*>(m_shader_program);
	std::string cur_material_path = Materials::get()->getMaterialPath(getName());
	cur_material_path = cur_material_path.substr(0, cur_material_path.rfind('/') + 1);
	std::string processed_cur_material_path = Utils::getPathXMLFromAbsolute(cur_material_path);

	pugi::xml_document mat_xml;
	pugi::xml_node xml_node_material = mat_xml.append_child("material");
	xml_node_material.append_attribute("name").set_value(m_name.c_str());

	for (int i = 0; i < m_vec_vertex_shaders.size(); ++i) {
		OpenGLShader* ogl_shader = dynamic_cast<OpenGLShader*>(m_vec_vertex_shaders[i]);
		pugi::xml_node xml_node_vertex_shdr = xml_node_material.append_child("shader");
		xml_node_vertex_shdr.append_attribute("type").set_value("vertex");
		if (ogl_shader_program->isShaderAttached(ogl_shader)) {
			std::string m_shader_path = m_vec_vertex_shader_paths[i];
			if (m_shader_path.empty()) {
				std::string shader_name = m_name + "_v.glsl";
				m_shader_path = processed_cur_material_path + shader_name;
				ogl_shader->save((cur_material_path + shader_name).c_str());
			}
			if (m_shader_path.empty()) {
				std::cerr << "Failed to save vertex shader path: " << m_name << std::endl;
			} else {
				xml_node_vertex_shdr.append_attribute("path").set_value(m_shader_path.c_str());
			}
		}
	}

	for (int i = 0; i < m_vec_fragment_shaders.size(); ++i) {
		OpenGLShader* ogl_shader = dynamic_cast<OpenGLShader*>(m_vec_fragment_shaders[i]);
		pugi::xml_node xml_node_fragment_shdr = xml_node_material.append_child("shader");
		xml_node_fragment_shdr.append_attribute("type").set_value("fragment");
		if (ogl_shader_program->isShaderAttached(ogl_shader)) {
			std::string m_shader_path = m_vec_fragment_shader_paths[i];
			if (m_shader_path.empty()) {
				std::string shader_name = m_name + "_f.glsl";
				m_shader_path = processed_cur_material_path + shader_name;
				ogl_shader->save((cur_material_path + shader_name).c_str());
			}
			if (m_shader_path.empty()) {
				std::cerr << "Failed to save fragment shader path: " << m_name << std::endl;
			} else {
				xml_node_fragment_shdr.append_attribute("path").set_value(m_shader_path.c_str());
			}
		}
	}

	for (int i = 0; i < m_shader_program->getNumVariables(); ++i) {
		pugi::xml_node xml_node_variable = xml_node_material.append_child("variable");
		xml_node_variable.append_attribute("name").set_value(m_shader_program->getVariableName(i));
		Variable* var = m_shader_program->getVariable(i);
		std::string type_name;
		if (var->isInt()) {
			type_name = "int";
		} else if (var->isFloat()) {
			type_name = "float";
		} else if (var->isVec3()) {
			type_name = "vec3";
		} else if (var->isVec4()) {
			type_name = "vec4";
		} else {
			xml_node_material.remove_child(xml_node_variable);
			continue;
		}
		xml_node_variable.append_attribute("type").set_value(type_name.c_str());
		xml_node_variable.append_child(pugi::node_pcdata).set_value(Utils::getStringVariable(var).c_str());
	}

	mat_xml.save_file(path);
}

int Material::load(const char* path) {
	remove_shader_program_and_shaders();
	initialize_shader_program_and_shaders();

	pugi::xml_document mat_xml;
	pugi::xml_parse_result xml_res = mat_xml.load_file(path);
	if (xml_res) {
		pugi::xml_node xml_node_material = mat_xml.child("material");
		if (xml_node_material) {
			setName(std::string(xml_node_material.attribute("name").value()));
			pugi::xml_node xml_node_vertex_shader = xml_node_material.find_child_by_attribute("shader", "type", "vertex");
			if (xml_node_vertex_shader) {
				std::string shdr_path = xml_node_vertex_shader.attribute("path").value();
				std::string abs_shdr_path = Utils::getAbsolutePathFromXML(shdr_path);
				if (abs_shdr_path.empty()) {
					std::cerr << "Material \'" << getName() << "\'Failed to specify shader path: " << shdr_path << std::endl;
					return -1;
				}
				addVertexShaderSource(abs_shdr_path.c_str());
			}
			pugi::xml_node xml_node_fragment_shader = xml_node_material.find_child_by_attribute("shader", "type", "fragment");
			if (xml_node_fragment_shader) {
				std::string shdr_path = xml_node_fragment_shader.attribute("path").value();
				std::string abs_shdr_path = Utils::getAbsolutePathFromXML(shdr_path);
				if (abs_shdr_path.empty()) {
					std::cerr << "Material \'" << getName() << "\'Failed to specify shader path: " << shdr_path << std::endl;
					return -1;
				}
				addFragmentShaderSource(abs_shdr_path.c_str());
			}

			for (pugi::xml_node variable_node = xml_node_material.child("variable");
				variable_node;
				variable_node = variable_node.next_sibling("variable")) {
				Variable* var = nullptr;
				Utils::parseXmlVariable(variable_node, &var);
				const char *var_name = variable_node.attribute("name").value();
				m_shader_program->setVariable(var_name, var);
				if (var) {
					delete var;
				}
			}

			for (pugi::xml_node texture_node = xml_node_material.child("texture");
				texture_node;
				texture_node = texture_node.next_sibling("texture")) {
				const char* tex_name = texture_node.attribute("name").value();
				Texture texture;

				if (!texture_node.attribute("path") || strlen(texture_node.attribute("path").value()) == 0) {
					// todo empty tex
				}
				else {
					const char* tex_path = texture_node.attribute("path").value();
					std::string abs_tex_path = Utils::getAbsolutePathFromXML(tex_path);
					if (abs_tex_path.empty()) {
						std::cerr << "Material \'" << getName() << "\'Failed to specify texture path: " << tex_path << std::endl;
						return -1;
					}
					texture.load(abs_tex_path.c_str());
					m_shader_program->setTexture(tex_name, texture);
				}
			}
			apply();

			return 1;
		}
	}
	return -1;
}

void Material::setName(std::string name)
{
	m_name = name;
}

const char* Material::getName()
{
	return m_name.c_str();
}

void Material::setVariable(const char* name, Variable *variable) {
	m_shader_program->setVariable(name, variable);
}

Variable *Material::getVariable(const char* name) {
	return m_shader_program->getVariable(name);
}

const char* Material::getVariableName(int index) {
	return m_shader_program->getVariableName(index);
}

int Material::getNumVariables() {
	return m_shader_program->getNumVariables();
}

Texture Material::getTexture(const char* name) {
	return m_shader_program->getTexture(name);
}

const char* Material::getTextureName(int index) {
	return m_shader_program->getTextureName(index);
}

void Material::setTexture(const char* name, Texture texture) {
	m_shader_program->setTexture(name, texture);
}

int Material::getNumTextures() {
	return m_shader_program->getNumTextures();
}

void Material::addVertexShaderSource(const char* path) {
	m_vec_vertex_shaders.emplace_back(new OpenGLShader(Shader::Type::VERTEX));
	m_vec_vertex_shaders.back()->loadSource(path);
	m_shader_program->addShader(m_vec_vertex_shaders.back());

	m_vec_vertex_shader_paths.push_back(Utils::getPathXMLFromAbsolute(path));
}

void Material::addVertexShaderContents(const char* code) {
	m_vec_vertex_shaders.emplace_back(new OpenGLShader(Shader::Type::VERTEX));
	m_vec_vertex_shaders.back()->loadContents(code);
	m_shader_program->addShader(m_vec_vertex_shaders.back());
}

void Material::addFragmentShaderSource(const char* path) {
	m_vec_fragment_shaders.emplace_back(new OpenGLShader(Shader::Type::FRAGMENT));
	m_vec_fragment_shaders.back()->loadSource(path);
	m_shader_program->addShader(m_vec_fragment_shaders.back());

	m_vec_fragment_shader_paths.push_back(Utils::getPathXMLFromAbsolute(path));
}

void Material::addFragmentShaderContents(const char* code) {
	m_vec_fragment_shaders.emplace_back(new OpenGLShader(Shader::Type::FRAGMENT));
	m_vec_fragment_shaders.back()->loadContents(code);
	m_shader_program->addShader(m_vec_fragment_shaders.back());
}

void Material::initialize_shader_program_and_shaders() {
	m_shader_program = new OpenGLShaderProgram();
}

void Material::remove_shader_program_and_shaders() {
	while (!m_vec_fragment_shaders.empty()) {
		Shader* shader = m_vec_fragment_shaders.back();
		m_vec_fragment_shaders.pop_back();
		delete shader;

		m_vec_fragment_shader_paths.pop_back();
	}

	while (!m_vec_vertex_shaders.empty()) {
		Shader* shader = m_vec_vertex_shaders.back();
		m_vec_vertex_shaders.pop_back();
		delete shader;

		m_vec_vertex_shader_paths.pop_back();
	}

	if (m_shader_program) {
		delete m_shader_program;
		m_shader_program = nullptr;
	}
}

void Material::apply() {
	dynamic_cast<OpenGLShaderProgram*>(m_shader_program)->link();
}

void Material::use() {
	m_shader_program->use();
}
