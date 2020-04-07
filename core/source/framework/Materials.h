#ifndef AETNA_MATERIALS_H
#define AETNA_MATERIALS_H

#include "Utils.h"

#include <map>
#include <iostream>
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
	
	void initialize(const char* path_to_materials) { // rename to "addMaterials" (or "findMaterials") (or "loadMaterials")
		for (const std::filesystem::directory_entry& entry : std::filesystem::recursive_directory_iterator(path_to_materials)) {
			if (entry.is_regular_file() && entry.path().extension() == ".mat") {
				pugi::xml_document mat_xml;
				pugi::xml_parse_result xml_res = mat_xml.load_file(entry.path().string().c_str());
				if (xml_res) {
					pugi::xml_node xml_node_material = mat_xml.child("material");
					if (xml_node_material) {
						std::string mat_name = xml_node_material.attribute("name").value();
						if (!mat_name.empty()) {
							// if (!m_map_ref_materials[mat_name])
							m_map_ref_materials[mat_name] = entry.path().string().c_str(); 
						}
					}
				}
			}
		}
	}
};

#endif //AETNA_MATERIALS_H