#ifndef AETNA_SHADER_H
#define AETNA_SHADER_H

#include <glm/glm.hpp>

#include <string>
#include <vector>

class Shader {
public:
	enum class Type {
		VERTEX = 0,
		FRAGMENT
	};

	Shader() : m_path(""), m_type() {

	}

	Shader(const char* path, Type type) {
		m_path = path;
		m_type = type;
	}

	const char* getPath() {
		return m_path.c_str();
	}

	Type getType() {
		return m_type;
	}

protected:
	Type m_type;
	std::string m_path;
};

#endif //AETNA_SHADER_H
