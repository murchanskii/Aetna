#include "Material.h"
#include "framework/shader/OpenGLShaderProgram.h"
#include "framework/Utils.h"

#include <pugixml.hpp>

#include <iostream>

Material::Material() : name("") {
	default_shader_program_used = true;
	m_shader_program = new OpenGLShaderProgram();

	load((Utils::getPathToCore() + "../materials/base_material.mat").c_str());
}

Material::~Material() {
	if (default_shader_program_used) {
		delete m_shader_program;
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
	xml_node_material.append_attribute("name").set_value(name.c_str());
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
		xml_node_variable.append_child(pugi::node_pcdata).set_value(getStringVariable(var).c_str());
	}

	mat_xml.save_file(path);
}

void Material::load(const char* path) {
	pugi::xml_document mat_xml;
	pugi::xml_parse_result xml_res = mat_xml.load_file(path);
	if (xml_res && m_shader_program) {
		pugi::xml_node xml_node_material = mat_xml.child("material");
		if (xml_node_material) {
			name = std::string(xml_node_material.attribute("name").value());
			pugi::xml_node xml_node_vertex_shader = xml_node_material.find_child_by_attribute("shader", "type", "vertex");
			if (xml_node_vertex_shader) {
				m_shader_program->setVertexShader((Utils::getPathToCore() + xml_node_vertex_shader.attribute("path").value()).c_str());
			}
			pugi::xml_node xml_node_fragment_shader = xml_node_material.find_child_by_attribute("shader", "type", "fragment");
			if (xml_node_fragment_shader) {
				m_shader_program->setFragmentShader((Utils::getPathToCore() + xml_node_fragment_shader.attribute("path").value()).c_str());
			}

			for (pugi::xml_node variable_node = xml_node_material.child("variable");
				variable_node;
				variable_node = variable_node.next_sibling("variable")) {
				parseXmlVariable(variable_node);
			}
		}
	}
}

void Material::apply() {
	m_shader_program->use();
}

std::string Material::getStringVariable(Variable* variable) {
	std::string result;

	if (variable->isInt()) {
		result = std::to_string(variable->getInt());
	} else if (variable->isFloat()) {
		result = std::to_string(variable->getFloat());
	} else if (variable->isVec3()) {
		for (int i = 0; i < variable->getVec3().length(); ++i) {
			result += std::to_string(variable->getVec3()[i]) + " ";
		}
		result.erase(result.end() - 1);
	} else if (variable->isVec4()) {
		for (int i = 0; i < variable->getVec4().length(); ++i) {
			result += std::to_string(variable->getVec4()[i]) + " ";
		}
		result.erase(result.end() - 1);
	}

	return result;
}

void Material::parseXmlVariable(pugi::xml_node variable_node) {
	pugi::xml_attribute var_name = variable_node.attribute("name");
	if (!var_name) {
		std::cerr << "Material::load: Unknown variable" << "\n";
		return;
	}
	pugi::xml_attribute var_type = variable_node.attribute("type");
	if (var_type) {
		if (strcmp(var_type.value(), "int") == 0) {
			m_shader_program->setVariable(var_name.value(),
				&VariableInt(parseXmlVariableInt(variable_node.child_value())));
		} else if (strcmp(var_type.value(), "float") == 0) {
			m_shader_program->setVariable(var_name.value(),
				&VariableFloat(parseXmlVariableFloat(variable_node.child_value())));
		} else if (strcmp(var_type.value(), "vec3") == 0) {
			m_shader_program->setVariable(var_name.value(),
				&VariableVec3(parseXmlVariableVec3(variable_node.child_value())));
		} else if (strcmp(var_type.value(), "vec4") == 0) {
			m_shader_program->setVariable(var_name.value(),
				&VariableVec4(parseXmlVariableVec4(variable_node.child_value())));
		} else {
			std::cerr << "Material::load: Unknown variable " << variable_node <<
				" type " << var_type.value() << "\n";
		}
	} else {
		std::cerr << "Material::load: Unknown variable " << variable_node << " type\n";
	}
}

int Material::parseXmlVariableInt(const char* str_var) {
	return std::stoi(str_var);
}

float Material::parseXmlVariableFloat(const char* str_var) {
	return std::stof(str_var);
}

glm::vec3 Material::parseXmlVariableVec3(const char* str_var) {
	glm::vec3 result = glm::vec3(0.0f);

	std::string std_str_var = str_var;
	std::string delimiter = " ";
	std::vector<std::string> digits = Utils::splitString(std_str_var, delimiter);
	if (digits.size() != 3) {
		return result;
	}

	for (int i = 0; i < digits.size(); ++i) {
		if (digits[i].back() == '0' ||
			digits[i].back() == '1' ||
			digits[i].back() == '2' ||
			digits[i].back() == '3' ||
			digits[i].back() == '4' ||
			digits[i].back() == '5' ||
			digits[i].back() == '6' ||
			digits[i].back() == '7' ||
			digits[i].back() == '8' ||
			digits[i].back() == '9' ||
			digits[i].back() == 'f') {
			if (digits[i].back() == 'f') {
				digits[i].erase(digits[i].end() - 1);
			}
			result[i] = std::stof(digits[i]);
		}
		else {
			return glm::vec3(0.0f);
		}
	}
	return result;
}

glm::vec4 Material::parseXmlVariableVec4(const char* str_var) {
	glm::vec4 result = glm::vec4(0.0f);

	std::string std_str_var = str_var;
	std::string delimiter = " ";
	std::vector<std::string> digits = Utils::splitString(std_str_var, delimiter);
	if (digits.size() != 4) {
		return result;
	}

	for (int i = 0; i < digits.size(); ++i) {
		if (digits[i].back() == '0' ||
			digits[i].back() == '1' ||
			digits[i].back() == '2' ||
			digits[i].back() == '3' ||
			digits[i].back() == '4' ||
			digits[i].back() == '5' ||
			digits[i].back() == '6' ||
			digits[i].back() == '7' ||
			digits[i].back() == '8' ||
			digits[i].back() == '9' ||
			digits[i].back() == 'f') {
			if (digits[i].back() == 'f') {
				digits[i].erase(digits[i].end() - 1);
			}
			result[i] = std::stof(digits[i]);
		} else {
			return glm::vec4(0.0f);
		}
	}
	return result;
}

