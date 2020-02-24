//
// Created by Candles on 19.01.2020.
//

#include "OpenGLShaderProgram.h"
#include <glm/gtc/type_ptr.hpp>
#include <sstream>
#include <fstream>
#include <iostream>

OpenGLShaderProgram::OpenGLShaderProgram() {
    m_program_id = glCreateProgram();
    glBindAttribLocation(m_program_id, 0, "pos"); // temp
    glBindAttribLocation(m_program_id, 1, "color"); // temp

    m_vertex_shader_id = 0;
    m_fragment_shader_id = 0;
}

OpenGLShaderProgram::~OpenGLShaderProgram() {

}

void OpenGLShaderProgram::setVertexShader(const char *path) {
    m_vertex_shader_id = create_shader(path, GL_VERTEX_SHADER);
    glAttachShader(m_program_id, m_vertex_shader_id);

    if (m_fragment_shader_id > 0) {
        glLinkProgram(m_program_id);
        check_program_linking(m_program_id);
    }
}

void OpenGLShaderProgram::setFragmentShader(const char *path) {
    m_fragment_shader_id = create_shader(path, GL_FRAGMENT_SHADER);
    glAttachShader(m_program_id, m_fragment_shader_id);

    if (m_vertex_shader_id > 0) {
        glLinkProgram(m_program_id);
        check_program_linking(m_program_id);
    }
}

void OpenGLShaderProgram::use() {
    glUseProgram(m_program_id);
}

GLuint OpenGLShaderProgram::getProgramID() {
    return m_program_id;
}

GLuint OpenGLShaderProgram::create_shader(const char *path_to_shader, GLuint shader_type) {
    std::ifstream shader_file(path_to_shader, std::ios::in);
    std::string shader_contents;

	if (shader_file.is_open()) {
		std::string line;
		while (std::getline(shader_file, line)) {
			shader_contents += "\n" + line;
		}
		shader_file.close();
	}
	else {
		std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	
    GLuint shader_id;
    shader_id = glCreateShader(shader_type);
    const char *shader_contents_ch = shader_contents.c_str();
    glShaderSource(shader_id, 1, &shader_contents_ch, nullptr);
    glCompileShader(shader_id);
    check_shader_compilation(shader_id);
    return shader_id;
}

void OpenGLShaderProgram::check_shader_compilation(GLuint &shader_id) {
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader_id, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" <<
                  infoLog << std::endl;
    }
    else {
        std::cout << "SUCCESS:: Shader has been compiled" << std::endl;
    }
}

void OpenGLShaderProgram::check_program_linking(GLuint &program_id) {
    GLint success;
    GLchar infoLog[512];
    glGetProgramiv(program_id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program_id, 512, nullptr, infoLog);
        std::cout << "ERROR::PROGRAM::LINKING_FAILED\n" <<
                  infoLog << std::endl;
    }
    else
        std::cout << "SUCCESS:: Program has been linked" << std::endl;
}

void OpenGLShaderProgram::uniform3fv(const char *var_name, glm::vec3 data) {
    GLint location = glGetUniformLocation(m_program_id, var_name);
    glUniform3fv(location, 1, glm::value_ptr(data));
}

void OpenGLShaderProgram::uniformMatrix4fv(const char *var_name, glm::mat4 data) {
    GLint location = glGetUniformLocation(m_program_id, var_name);
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(data));
}

void OpenGLShaderProgram::uniform1f(const char *var_name, GLfloat data) {
    GLint location = glGetUniformLocation(m_program_id, var_name);
    glUniform1f(location, data);
}

void OpenGLShaderProgram::uniform1i(const char *var_name, GLint data) {
    GLint location = glGetUniformLocation(m_program_id, var_name);
    glUniform1i(location, data);
}
