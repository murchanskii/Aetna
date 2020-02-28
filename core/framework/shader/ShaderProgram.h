//
// Created by Candles on 19.01.2020.
//

#ifndef AETNA_SHADERPROGRAM_H
#define AETNA_SHADERPROGRAM_H

#include "../Utils.h"
#include <vector>

class ShaderProgram {
public:
    virtual void setVertexShader(const char *path) = 0;
    virtual void setFragmentShader(const char *path) = 0;
	virtual bool isVertexShaderBinded() = 0;
	virtual bool isFragmentShaderBinded() = 0;
	virtual const char* getVertexShaderPath() = 0;
	virtual const char* getFragmentShaderPath() = 0;
    virtual void use() = 0;

	virtual void setVariable(std::string name, Variable *var) = 0;
	virtual Variable* getVariable(std::string name) = 0; 
	virtual Variable* getVariable(int index) = 0;
	virtual std::string getVariableName(int index) = 0;
	virtual int getNumVariables() = 0;
};


#endif //AETNA_SHADERPROGRAM_H
