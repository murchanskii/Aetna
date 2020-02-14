//
// Created by Candles on 19.01.2020.
//

#ifndef AETNA_OPENGLSHADERPROGRAM_H
#define AETNA_OPENGLSHADERPROGRAM_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "ShaderProgram.h"

#include <glm/glm.hpp>

#include <string>

class OpenGLShaderProgram : public ShaderProgram {
public:
    OpenGLShaderProgram();
    virtual ~OpenGLShaderProgram();

    void setVertexShader(const char *path) override;
    void setFragmentShader(const char *path) override;
    void use() override;

    GLuint getProgramID();

    void uniform3fv(const char *var_name, glm::vec3 data);
    void uniformMatrix4fv(const char *var_name, glm::mat4 data);
    void uniform1f(const char *var_name, GLfloat data);
    void uniform1i(const char *var_name, GLint data);

private:
    GLuint m_program_id;
    GLuint m_vertex_shader_id;
    GLuint m_fragment_shader_id;

    GLuint create_shader(const char *path_to_shader, GLuint shader_type);
    void check_shader_compilation(GLuint &shader_id);
    void check_program_linking(GLuint &program_id);

};


#endif //AETNA_OPENGLSHADERPROGRAM_H
