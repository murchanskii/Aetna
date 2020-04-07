#include "Material.h"
#include <framework/Materials.h>
#include <framework/render/shader/OpenGLShaderProgram.h>
#include <Utils.h>
#include <framework/Engine.h>

#include <pugixml.hpp>

#include <iostream>

Material::Material() {
	default_shader_program_used = true;
	m_shader_program = new OpenGLShaderProgram();
	m_vec_textures.emplace_back(new Texture());
}

Material::~Material() {
	if (default_shader_program_used) {
		delete m_shader_program;
	}

	while (!m_vec_textures.empty()) {
		Texture* texture = m_vec_textures[0];
		m_vec_textures.erase(m_vec_textures.begin());
		delete texture;
	}
}

ShaderProgram *Material::getShaderProgram() {
	return m_shader_program;
}

void Material::setShaderProgram(ShaderProgram* shdr_prog) {
	default_shader_program_used = false;
	m_shader_program = shdr_prog;
}

void Material::save(const char* path) {
	pugi::xml_document mat_xml;
	pugi::xml_node xml_node_material = mat_xml.append_child("material");
	xml_node_material.append_attribute("name").set_value(m_name.c_str());
	pugi::xml_node xml_node_vertex_shdr = xml_node_material.append_child("shader");
	xml_node_vertex_shdr.append_attribute("type").set_value("vertex");
	if (m_shader_program->isVertexShaderBinded()) {
		xml_node_vertex_shdr.append_attribute("path").set_value(m_shader_program->getVertexShaderPath());
	}
	pugi::xml_node xml_node_fragment_shdr = xml_node_material.append_child("shader");
	xml_node_fragment_shdr.append_attribute("type").set_value("fragment");
	if (m_shader_program->isFragmentShaderBinded()) {
		xml_node_fragment_shdr.append_attribute("path").set_value(m_shader_program->getFragmentShaderPath());
	}

	for (int i = 0; i < m_shader_program->getNumVariables(); ++i) {
		pugi::xml_node xml_node_variable = xml_node_material.append_child("variable");
		xml_node_variable.append_attribute("name").set_value(m_shader_program->getVariableName(i).c_str());
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
	auto remove_shader_program = [&]() {
		delete m_shader_program;
		m_shader_program = nullptr;
		default_shader_program_used = false; 
	};

	if (m_shader_program) {
		remove_shader_program();
	}

	m_shader_program = new OpenGLShaderProgram();
	default_shader_program_used = true;

	pugi::xml_document mat_xml;
	pugi::xml_parse_result xml_res = mat_xml.load_file(path);
	if (xml_res) {
		pugi::xml_node xml_node_material = mat_xml.child("material");
		if (xml_node_material) {
			setName(std::string(xml_node_material.attribute("name").value()));
			pugi::xml_node xml_node_vertex_shader = xml_node_material.find_child_by_attribute("shader", "type", "vertex");
			if (xml_node_vertex_shader) {
				m_shader_program->setVertexShader((std::string(Engine::get()->getCorePath()) + xml_node_vertex_shader.attribute("path").value()).c_str());
				if (strcmp(m_shader_program->getVertexShaderPath(), "") == 0) {
					remove_shader_program();
					return -1;
				}
			}
			pugi::xml_node xml_node_fragment_shader = xml_node_material.find_child_by_attribute("shader", "type", "fragment");
			if (xml_node_fragment_shader) {
				m_shader_program->setFragmentShader((std::string(Engine::get()->getCorePath()) + xml_node_fragment_shader.attribute("path").value()).c_str());
				if (strcmp(m_shader_program->getFragmentShaderPath(), "") == 0) {
					remove_shader_program();
					return -1;
				}
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

			for (pugi::xml_node variable_node = xml_node_material.child("texture");
				variable_node;
				variable_node = variable_node.next_sibling("texture")) {
				const char* tex_name = variable_node.attribute("name").value();
				const char* tex_path = variable_node.attribute("path").value();

				// test
				m_vec_textures[0]->load(tex_path);
			}

			return 1;
		}
	}
	remove_shader_program();
	return -1;
}

void Material::setName(std::string name)
{
	m_name = name;
}

std::string Material::getName()
{
	return m_name;
}

Texture* Material::getTextureAlbedo() {
	if (m_vec_textures.empty()) {
		return nullptr;
	}
	// todo: find proper texture
	return m_vec_textures[0];
}

void Material::apply() {
	m_shader_program->use();
}
