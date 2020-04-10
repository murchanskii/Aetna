#ifndef AETNA_OPENGLSHADERPROGRAM_H
#define AETNA_OPENGLSHADERPROGRAM_H

#include <framework/render/shader/ShaderProgram.h>
#include <framework/render/shader/OpenGLShader.h>

class OpenGLShaderProgram : public ShaderProgram {
public:
    OpenGLShaderProgram();
    virtual ~OpenGLShaderProgram();

    void addShader(Shader* shader) override;
    void addShader(OpenGLShader* shader);
    void removeShader(Shader *shader) override;
    void removeShader(OpenGLShader* shader);
    bool isShaderAttached(OpenGLShader* shader);
    void link();
    void use() override;

    int getProgramID();

    void setVariable(const char* name, Variable *var) override;
    Variable *getVariable(const char* name) override;
    Variable *getVariable(int index) override;
    int findVariable(const char* name) override;
    const char* getVariableName(int index) override;
    int getNumVariables() override;

    void setTexture(const char* name, Texture var) override;
    Texture getTexture(const char* name) override;
    Texture getTexture(int index) override;
    int findTexture(const char* name) override;
    const char* getTextureName(int index) override;
    int getNumTextures() override;
    
private:
    int m_program_id;
    std::vector<OpenGLShader*> m_vec_shaders;

    int find_shader(OpenGLShader* shader);
    void check_program_linking(int &program_id);

    struct VariableInfo {
        std::string name;
        Variable *data;
    };

    struct TextureInfo {
        unsigned int location;
        std::string name;
        Texture *data;
    };

    std::vector<VariableInfo> m_variables;
    std::vector<TextureInfo> m_textures;
};


#endif //AETNA_OPENGLSHADERPROGRAM_H
