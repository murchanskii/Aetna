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

	Shader(Type type) {
		m_type = type;
	}

	Type getType() {
		return m_type;
	}

	virtual void save(const char* path) = 0;
	virtual void loadSource(const char* path) = 0;
	virtual void loadContents(const char *code) = 0;

protected:
	Type m_type;
	std::string m_contents;
};

#endif //AETNA_SHADER_H
