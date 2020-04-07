//
// Created by Candles on 19.01.2020.
//

#ifndef AETNA_OPENGLSHADERPROGRAM_H
#define AETNA_OPENGLSHADERPROGRAM_H

#include <framework/render/shader/ShaderProgram.h>
#include <framework/render/shader/OpenGLShader.h>

#include <string>
#include <map>

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

    void setVariable(std::string name, Variable* var) override;
    Variable* getVariable(std::string name) override;
    Variable* getVariable(int index) override;
    std::string getVariableName(int index) override;
    int getNumVariables() override;
    
private:
    int m_program_id;
    OpenGLShader* m_vertex_shader;
    OpenGLShader* m_fragment_shader;

    void check_program_linking(int &program_id);
    int get_var_index_by_name(std::string name);

    std::vector<std::pair<std::string, Variable*>> m_variables;
};


#endif //AETNA_OPENGLSHADERPROGRAM_H
