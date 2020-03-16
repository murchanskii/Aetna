#ifndef AETNA_MATERIALS_H
#define AETNA_MATERIALS_H

#include "Utils.h"

#include <iostream>
#include <map>
#include <string>
#include <filesystem>

class Materials {
private:
	Materials() { }
	virtual ~Materials() { }

	std::map<std::string, std::string> m_map_ref_materials;

public:
	static Materials* get() {
		static Materials instance;
		return &instance;
	}

	Materials(Materials const&) = delete;
	Materials& operator=(Materials const&) = delete;

	std::string getMaterialPath(const char* mat_name) {
		return m_map_ref_materials[mat_name];
	}
	
	void initialize(const char* path_to_materials) {
		for (const std::filesystem::directory_entry& entry : std::filesystem::recursive_directory_iterator(path_to_materials)) {
			if (entry.is_regular_file() && entry.path().extension() == ".mat") {
				pugi::xml_document mat_xml;
				pugi::xml_parse_result xml_res = mat_xml.load_file(entry.path().string().c_str());
				if (xml_res) {
					pugi::xml_node xml_node_material = mat_xml.child("material");
					if (xml_node_material) {
						std::string mat_name = xml_node_material.attribute("name").value();
						if (!mat_name.empty()) {
							m_map_ref_materials[mat_name] = entry.path().string().c_str();
						}
					}
				}
			}
		}
	}

	static void parseXmlVariable(pugi::xml_node variable_node, Variable **var) {
		pugi::xml_attribute var_name = variable_node.attribute("name");
		if (!var_name) {
			std::cerr << "Material::load: Unknown variable" << "\n";
			return;
		}
		pugi::xml_attribute var_type = variable_node.attribute("type");
		if (var_type) {
			if (strcmp(var_type.value(), "int") == 0) {
				*var = new VariableInt(Utils::stringToInt(variable_node.child_value()));
			} else if (strcmp(var_type.value(), "float") == 0) {
				*var = new VariableFloat(Utils::stringToFloat(variable_node.child_value()));
			} else if (strcmp(var_type.value(), "vec3") == 0) {
				*var = new VariableVec3(Utils::stringToVec3(variable_node.child_value()));
			} else if (strcmp(var_type.value(), "vec4") == 0) {
				*var = new VariableVec4(Utils::stringToVec4(variable_node.child_value()));
			}
			else {
				std::cerr << "Material::load: Unknown variable " << variable_node <<
					" type " << var_type.value() << "\n";
				return;
			}
		}
		else {
			std::cerr << "Material::load: Unknown variable " << variable_node << " type\n";
		}
	}
};

#endif //AETNA_MATERIALS_H