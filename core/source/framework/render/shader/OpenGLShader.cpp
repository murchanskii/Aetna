#include "OpenGLShader.h"

#include <glad/glad.h>
#include <Utils.h>

#include <iostream>

OpenGLShader::OpenGLShader(Type type) : Shader(type), m_id(-1) {
	m_type = -1;
	switch (type) {
	case Shader::Type::VERTEX:
		m_type = GL_VERTEX_SHADER;
		break;
	case Shader::Type::FRAGMENT:
		m_type = GL_FRAGMENT_SHADER;
		break;
	default:
		break;
	}
}

void OpenGLShader::check_shader_compilation(int& shader_id) {
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader_id, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" <<
			infoLog << std::endl;
	} else {
		std::cout << "SUCCESS:: Shader has been compiled" << std::endl;
	}
}

void OpenGLShader::save(const char* path) {
	Utils::saveFile(path, m_contents, false);
}

void OpenGLShader::loadSource(const char* path) {
	m_contents = Utils::readFile(path);
	loadContents(m_contents.c_str());
}

void OpenGLShader::loadContents(const char* code) {
	if (strlen(code) == 0) {
		std::cerr << "Failed to load shader contents: empty()" << std::endl;
		return;
	}

	m_contents = code;
	m_id = glCreateShader(m_type);
	glShaderSource(m_id, 1, &code, nullptr);
	glCompileShader(m_id);
	check_shader_compilation(m_id);
}

int OpenGLShader::getID() {
	return m_id;
}