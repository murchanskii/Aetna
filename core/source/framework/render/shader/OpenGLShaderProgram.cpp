#include "OpenGLShaderProgram.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

OpenGLShaderProgram::OpenGLShaderProgram() {
    m_program_id = glCreateProgram();
}

OpenGLShaderProgram::~OpenGLShaderProgram() {
    for (VariableInfo &var : m_variables) {
        if (var.data) {
            delete var.data;
            var.data = nullptr;
        }
    }

    for (int i = 0; i < m_vec_shaders.size(); ++i) {
        removeShader(m_vec_shaders[i]);
    }
}

void OpenGLShaderProgram::addShader(Shader* shader) {
    OpenGLShader* ogl_shader = nullptr;
    if (shader && (ogl_shader = dynamic_cast<OpenGLShader*>(shader))) {
        m_vec_shaders.emplace_back(ogl_shader);
        glAttachShader(m_program_id, m_vec_shaders.back()->getID());
    }
}

void OpenGLShaderProgram::addShader(OpenGLShader* shader) {
    m_vec_shaders.emplace_back(shader);
    glAttachShader(m_program_id, m_vec_shaders.back()->getID());
}

void OpenGLShaderProgram::removeShader(Shader* shader) {
    OpenGLShader* ogl_shader = nullptr;
    if (shader && (ogl_shader = dynamic_cast<OpenGLShader*>(shader))) {
        if (isShaderAttached(ogl_shader)) {
            glDetachShader(m_program_id, ogl_shader->getID());
        }
    }
}

void OpenGLShaderProgram::removeShader(OpenGLShader* shader) {
    if (isShaderAttached(shader)) {
        glDetachShader(m_program_id, shader->getID());
    }
}

bool OpenGLShaderProgram::isShaderAttached(OpenGLShader* shader) {
    int shader_index = find_shader(shader);
    if (shader_index >= 0) {
        return true;
    }
	return false;
}

void OpenGLShaderProgram::link() {
    glLinkProgram(m_program_id);
    check_program_linking(m_program_id);
}

void OpenGLShaderProgram::use() {
    for (int i = GL_TEXTURE0, tex_index = 0; tex_index < m_textures.size() && i <= GL_TEXTURE31; ++i, ++tex_index) {
        glActiveTexture(i);
        glBindTexture(GL_TEXTURE_2D, m_textures[tex_index].location);
    }

    glUseProgram(m_program_id);

    for (VariableInfo &var : m_variables) {
        if (var.data->isInt()) {
            GLint location = glGetUniformLocation(m_program_id, var.name.c_str());
            glUniform1i(location, var.data->getInt());
        } else if (var.data->isFloat()) {
            GLint location = glGetUniformLocation(m_program_id, var.name.c_str());
            glUniform1f(location, var.data->getFloat());
        } else if (var.data->isVec3()) {
            GLint location = glGetUniformLocation(m_program_id, var.name.c_str());
            glUniform3fv(location, 1, glm::value_ptr(var.data->getVec3()));
        } else if (var.data->isVec4()) {
            GLint location = glGetUniformLocation(m_program_id, var.name.c_str());
            glUniform4fv(location, 1, glm::value_ptr(var.data->getVec4()));
        } else if (var.data->isMat4()) {
            GLint location = glGetUniformLocation(m_program_id, var.name.c_str());
            glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(var.data->getMat4()));
        }
    }
}

int OpenGLShaderProgram::getProgramID() {
    return m_program_id;
}

int OpenGLShaderProgram::find_shader(OpenGLShader* shader) {
    for (int i = 0; i < m_vec_shaders.size(); ++i) {
        if (m_vec_shaders[i]->getID() == shader->getID()) {
            return i;
        }
    }
    return -1;
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

int OpenGLShaderProgram::findVariable(const char* name) {
    for (int i = 0; i < m_variables.size(); ++i) {
        if (strcmp(m_variables[i].name.c_str(), name) == 0) {
            return i;
        }
    }
    return -1;
}

void OpenGLShaderProgram::setVariable(const char* name, Variable *var) {
    int var_index = findVariable(name);
    if (var_index >= 0) {
        if (var->isInt()) {
            m_variables[var_index].data->setInt(var->getInt());
        } else if (var->isFloat()) {
            m_variables[var_index].data->setFloat(var->getFloat());
        } else if (var->isVec3()) {
            m_variables[var_index].data->setVec3(var->getVec3());
        } else if (var->isVec4()) {
            m_variables[var_index].data->setVec4(var->getVec4());
        } else if (var->isMat4()) {
            m_variables[var_index].data->setMat4(var->getMat4());
        }
    }
    else {
        m_variables.emplace_back();
        m_variables.back().name = name;

        if (var->isInt()) {
            m_variables.back().data = new VariableInt(var->getInt());
        } else if (var->isFloat()) {
            m_variables.back().data = new VariableFloat(var->getFloat());
        } else if (var->isVec3()) {
            m_variables.back().data = new VariableVec3(var->getVec3());
        } else if (var->isVec4()) {
            m_variables.back().data = new VariableVec4(var->getVec4());
        } else if (var->isMat4()) {
            m_variables.back().data = new VariableMat4(var->getMat4());
        }
    }
}

Variable *OpenGLShaderProgram::getVariable(const char* name) {
    int var_index = findVariable(name);
    if (var_index >= 0) {
        return m_variables[var_index].data;
    }
    return nullptr;
}

Variable *OpenGLShaderProgram::getVariable(int index) {
    return m_variables[index].data;
}

const char* OpenGLShaderProgram::getVariableName(int index) {
    return m_variables[index].name.c_str();
}

int OpenGLShaderProgram::getNumVariables() {
    return m_variables.size();
}

void OpenGLShaderProgram::setTexture(const char* name, Texture var) {
    int texture_index = findTexture(name);
    if (texture_index >= 0) {
        m_textures[texture_index].data = &var;
    }
    else {
        m_textures.emplace_back();
        TextureInfo* tex_info = &(m_textures.back());
        tex_info->name = name;
        tex_info->data = new Texture(var);

        glGenTextures(1, &(tex_info->location));
        glBindTexture(GL_TEXTURE_2D, tex_info->location);
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // load image, create texture and generate mipmaps
        if (tex_info->data->getData()) {
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex_info->data->getWidth(), tex_info->data->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_info->data->getData());
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        setVariable(("material." + tex_info->name).c_str(), &VariableInt(m_textures.size() - 1));
    }
}

Texture OpenGLShaderProgram::getTexture(const char* name) {
    int texture_index = findTexture(name);
    if (texture_index >= 0) {
        return getTexture(texture_index);
    }
    return Texture();
}

Texture OpenGLShaderProgram::getTexture(int index) {
    if (index >= 0) {
        return *(m_textures[index].data);
    }
    return Texture();
}

int OpenGLShaderProgram::findTexture(const char* name) {
    for (int i = 0; i < m_textures.size(); ++i) {
        if (strcmp(m_textures[i].name.c_str(), name) == 0) {
            return i;
        }
    }
    return -1;
}

const char* OpenGLShaderProgram::getTextureName(int index) {
    return m_textures[index].name.c_str();
}

int OpenGLShaderProgram::getNumTextures() {
    return m_textures.size();
}
