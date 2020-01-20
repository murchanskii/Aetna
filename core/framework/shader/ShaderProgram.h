//
// Created by Candles on 19.01.2020.
//

#ifndef AETNA_SHADERPROGRAM_H
#define AETNA_SHADERPROGRAM_H


class ShaderProgram {
public:
    virtual void setVertexShader(const char *path) = 0;
    virtual void setFragmentShader(const char *path) = 0;
    virtual void use() = 0;
};


#endif //AETNA_SHADERPROGRAM_H
