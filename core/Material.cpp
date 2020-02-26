#include "Material.h"
#include "framework/shader/OpenGLShaderProgram.h"
#include "framework/Utils.h"

#include "../3rd/pugixml/pugixml.hpp"

Material::Material() {
	default_shader_program_used = true;
	m_shader_program = new OpenGLShaderProgram();

	load((Utils::getPathToCore() + "../materials/base_material.mat").c_str());
}

Material::~Material() {
	if (default_shader_program_used) {
		delete m_shader_program;
	}
}

ShaderProgram const* Material::getShaderProgram() {
	return m_shader_program;
}

void Material::setShaderProgram(ShaderProgram* shdr_prog) {
	default_shader_program_used = false;
	m_shader_program = shdr_prog;
}

void Material::save(const char* path) {
	pugi::xml_document mat_xml;
	pugi::xml_node xml_node_material = mat_xml.append_child("material");
	pugi::xml_node xml_node_vertex_shdr = xml_node_material.append_child("shader");
	xml_node_vertex_shdr.append_attribute("type").set_value("vertex");
	xml_node_vertex_shdr.append_attribute("path");
	pugi::xml_node xml_node_fragment_shdr = xml_node_material.append_child("shader");
	xml_node_fragment_shdr.append_attribute("type").set_value("fragment");
	xml_node_fragment_shdr.append_attribute("path");

	mat_xml.save_file(path);
}

void Material::load(const char* path) {
	pugi::xml_document mat_xml;
	pugi::xml_parse_result xml_res = mat_xml.load_file(path);
	if (xml_res && m_shader_program) {
		pugi::xml_node xml_node_material = mat_xml.child("material");
		if (xml_node_material) {
			pugi::xml_node xml_node_vertex_shader = xml_node_material.find_child_by_attribute("shader", "type", "vertex");
			if (xml_node_vertex_shader) {
				m_shader_program->setVertexShader((Utils::getPathToCore() + xml_node_vertex_shader.attribute("path").value()).c_str());
			}
			pugi::xml_node xml_node_fragment_shader = xml_node_material.find_child_by_attribute("shader", "type", "fragment");
			if (xml_node_fragment_shader) {
				m_shader_program->setFragmentShader((Utils::getPathToCore() + xml_node_fragment_shader.attribute("path").value()).c_str());
			}
		}
	}
}

void Material::apply() {
	m_shader_program->use();
}
