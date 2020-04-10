#ifndef AETNA_SHADERPROGRAM_H
#define AETNA_SHADERPROGRAM_H

#include <framework/render/shader/Shader.h>
#include <Texture.h>
#include <Utils.h>
#include <vector>

class ShaderProgram {
public:
    virtual void addShader(Shader *shader) = 0;
    virtual void removeShader(Shader* shader) = 0;
    virtual void use() = 0;

    virtual void setVariable(const char* name, Variable *var) = 0;
    virtual Variable *getVariable(const char* name) = 0;
    virtual Variable *getVariable(int index) = 0;
    virtual int findVariable(const char* name) = 0;
    virtual const char* getVariableName(int index) = 0;
    virtual int getNumVariables() = 0;

    virtual void setTexture(const char* name, Texture tex) = 0;
    virtual Texture getTexture(const char* name) = 0;
    virtual Texture getTexture(int index) = 0;
    virtual int findTexture(const char* name) = 0;
    virtual const char* getTextureName(int index) = 0;
    virtual int getNumTextures() = 0;
};


#endif //AETNA_SHADERPROGRAM_H
