//
// Created by Candles on 19.01.2020.
//

#include "OpenGLShaderProgram.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

OpenGLShaderProgram::OpenGLShaderProgram() {
    m_program_id = glCreateProgram();
    glBindAttribLocation(m_program_id, 0, "pos"); // temp
    glBindAttribLocation(m_program_id, 1, "color"); // temp

    m_vertex_shader = nullptr;
    m_fragment_shader = nullptr;
}

OpenGLShaderProgram::~OpenGLShaderProgram() {
    for (std::pair<std::string, Variable*> var : m_variables) {
        delete var.second;
        var.second = nullptr;
    }

    if (m_vertex_shader) {
        delete m_vertex_shader;
        m_vertex_shader = nullptr;
    }

    if (m_fragment_shader) {
        delete m_fragment_shader;
        m_fragment_shader = nullptr;
    }
}

void OpenGLShaderProgram::setVertexShader(const char *path) {
    m_vertex_shader = new OpenGLShader(path, OpenGLShader::Type::VERTEX);
    glAttachShader(m_program_id, m_vertex_shader->getID());

    if (m_fragment_shader && m_fragment_shader->getID() > 0) {
        glLinkProgram(m_program_id);
        check_program_linking(m_program_id);
    }
}

void OpenGLShaderProgram::setFragmentShader(const char *path) {
    m_fragment_shader = new OpenGLShader(path, OpenGLShader::Type::FRAGMENT);
    glAttachShader(m_program_id, m_fragment_shader->getID());

    if (m_vertex_shader && m_vertex_shader->getID() > 0) {
        glLinkProgram(m_program_id);
        check_program_linking(m_program_id);
    }
}

bool OpenGLShaderProgram::isVertexShaderBinded() {
    return m_vertex_shader;
}

bool OpenGLShaderProgram::isFragmentShaderBinded() {
    return m_fragment_shader;
}

const char* OpenGLShaderProgram::getVertexShaderPath() {
    if (!m_vertex_shader) {
        return "";
    }
    return m_vertex_shader->getPath();
}

const char* OpenGLShaderProgram::getFragmentShaderPath() {
    if (!m_fragment_shader) {
        return "";
    }
    return m_fragment_shader->getPath();
}

void OpenGLShaderProgram::use() {
    glUseProgram(m_program_id);
    for (std::pair<std::string, Variable*> var : m_variables) {
        if (var.second->isInt()) {
            GLint location = glGetUniformLocation(m_program_id, var.first.c_str());
            glUniform1i(location, var.second->getInt());
        } else if (var.second->isFloat()) {
            GLint location = glGetUniformLocation(m_program_id, var.first.c_str());
            glUniform1f(location, var.second->getFloat());
        } else if (var.second->isVec3()) {
            GLint location = glGetUniformLocation(m_program_id, var.first.c_str());
            glUniform3fv(location, 1, glm::value_ptr(var.second->getVec3()));
        } else if (var.second->isVec4()) {
            GLint location = glGetUniformLocation(m_program_id, var.first.c_str());
            glUniform4fv(location, 1, glm::value_ptr(var.second->getVec4()));
        } else if (var.second->isMat4()) {
            GLint location = glGetUniformLocation(m_program_id, var.first.c_str());
            glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(var.second->getMat4()));
        }
    }
}

int OpenGLShaderProgram::getProgramID() {
    return m_program_id;
}

void OpenGLShaderProgram::check_program_linking(int &program_id) {
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

int OpenGLShaderProgram::get_var_index_by_name(std::string name) {
    for (int i = 0; i < m_variables.size(); ++i) {
        if (name == m_variables[i].first) {
            return i;
        }
    }
    return -1;
}

void OpenGLShaderProgram::setVariable(std::string name, Variable* var) {
    int var_index = get_var_index_by_name(name);
    if (var_index >= 0) {
        delete m_variables[var_index].second;
        m_variables[var_index].second = nullptr;
        m_variables.erase(m_variables.begin() + var_index);
    }

    m_variables.push_back(std::pair<std::string, Variable*>(name, nullptr));
    var_index = m_variables.size() - 1;

    if (var->isInt()) {
        m_variables[var_index].second = new VariableInt(var->getInt());
    } else if (var->isFloat()) {
        m_variables[var_index].second = new VariableFloat(var->getFloat());
    } else if (var->isVec3()) {
        m_variables[var_index].second = new VariableVec3(var->getVec3());
    } else if (var->isVec4()) {
        m_variables[var_index].second = new VariableVec4(var->getVec4());
    } else if (var->isMat4()) {
        m_variables[var_index].second = new VariableMat4(var->getMat4());
    }
}

Variable* OpenGLShaderProgram::getVariable(std::string name) {
    int var_index = get_var_index_by_name(name);
    if (var_index >= 0) {
        return m_variables[var_index].second;
    }
    return nullptr;
}

Variable* OpenGLShaderProgram::getVariable(int index) {
    return m_variables[index].second;
}

std::string OpenGLShaderProgram::getVariableName(int index) {
    return m_variables[index].first;
}

int OpenGLShaderProgram::getNumVariables() {
    return m_variables.size();
}
