//
// Created by Candles on 19.01.2020.
//

#ifndef AETNA_OPENGLSHADERPROGRAM_H
#define AETNA_OPENGLSHADERPROGRAM_H

#include "ShaderProgram.h"
#include "OpenGLShader.h"

#include <string>

class OpenGLShaderProgram : public ShaderProgram {
public:
    OpenGLShaderProgram();
    virtual ~OpenGLShaderProgram();

    void setVertexShader(const char *path) override;
    void setFragmentShader(const char *path) override;
    bool isVertexShaderBinded() override;
    bool isFragmentShaderBinded() override;
    const char* getVertexShaderPath() override;
    const char* getFragmentShaderPath() override;
    void use() override;

    int getProgramID();

    void setVariable(const char* name, Variable* var) override;
    
private:
    int m_program_id;
    OpenGLShader* m_vertex_shader;
    OpenGLShader* m_fragment_shader;

    void check_program_linking(int &program_id);

};


#endif //AETNA_OPENGLSHADERPROGRAM_H
