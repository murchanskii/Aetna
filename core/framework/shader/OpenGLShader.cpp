#include "OpenGLShader.h"

#include <glad/glad.h>

#include <sstream>
#include <fstream>
#include <iostream>

OpenGLShader::OpenGLShader(const char* path, Type type) {
	std::ifstream shader_file(path, std::ios::in);
	std::string shader_contents;

	if (shader_file.is_open()) {
		std::string line;
		while (std::getline(shader_file, line)) {
			shader_contents += "\n" + line;
		}
		shader_file.close();
	} else {
		std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	int shader_type = -1;
	switch (type) {
	case Shader::Type::VERTEX:
		shader_type = GL_VERTEX_SHADER;
		break;
	case Shader::Type::FRAGMENT:
		shader_type = GL_FRAGMENT_SHADER;
		break;
	default:
		break;
	}

	id = glCreateShader(shader_type);
	const char* shader_contents_ch = shader_contents.c_str();
	glShaderSource(id, 1, &shader_contents_ch, nullptr);
	glCompileShader(id);
	check_shader_compilation(id);
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

int OpenGLShader::getID() {
	return id;
}