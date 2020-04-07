#ifndef AETNA_OPENGLSHADER_H
#define AETNA_OPENGLSHADER_H

#include <framework/render/shader/Shader.h>

class OpenGLShader : public Shader {
public:
	OpenGLShader(const char* path, Type type);
	int getID();
private:
	int id;

	void check_shader_compilation(int& shader_id);
};

#endif //AETNA_OPENGLSHADER_H