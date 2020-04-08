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
	
	tex_albedo.texture = new Texture();
	tex_normal.texture = new Texture();
	tex_specular.texture = new Texture();
}

Material::~Material() {
	if (default_shader_program_used) {
		delete m_shader_program;
	}

	if (tex_albedo.texture) {
		delete tex_albedo.texture;
	}

	if (tex_normal.texture) {
		delete tex_normal.texture;
	}

	if (tex_specular.texture) {
		delete tex_specular.texture;
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

			for (pugi::xml_node texture_node = xml_node_material.child("texture");
				texture_node;
				texture_node = texture_node.next_sibling("texture")) {
				const char* tex_name = texture_node.attribute("name").value();
				const char* tex_type = texture_node.attribute("type").value();
				const char* tex_path = texture_node.attribute("path").value();

				TextureVariable* tex_to_load = nullptr;
				if (strcmp(tex_type, "albedo") == 0) {
					tex_to_load = &tex_albedo;
				} else if (strcmp(tex_type, "normal") == 0) {
					tex_to_load = &tex_normal;
				} else if (strcmp(tex_type, "specular") == 0) {
					tex_to_load = &tex_specular;
				}

				if (!tex_to_load) {
					std::cerr << "Texture type: " << tex_type << " unknown" << std::endl;
					continue;
				}

				tex_to_load->name = tex_name;
				tex_to_load->texture->load(tex_path);
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

Texture* Material::getTexture(TextureType type) {
	switch (type) {
	case Material::TextureType::ALBEDO:
		return tex_albedo.texture;
	case Material::TextureType::NORMAL:
		return tex_normal.texture;
	case Material::TextureType::SPECULAR:
		return tex_specular.texture;
	default:
		return nullptr;
	}
}

const char* Material::getTextureName(TextureType type) {
	std::string *result;
	switch (type) {
	case Material::TextureType::ALBEDO:
		result = &tex_albedo.name;
		break;
	case Material::TextureType::NORMAL:
		result = &tex_normal.name;
		break;
	case Material::TextureType::SPECULAR:
		result = &tex_specular.name;
		break;
	}
	return result->c_str();
}

void Material::setTexture(TextureType type, const char* path) {

}

void Material::setTexture(TextureType type, Texture* texture) {

}

void Material::apply() {
	m_shader_program->use();
}
