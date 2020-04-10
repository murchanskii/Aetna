#ifndef AETNA_OPENGLSHADER_H
#define AETNA_OPENGLSHADER_H

#include <framework/render/shader/Shader.h>

class OpenGLShader : public Shader {
public:
	OpenGLShader(Type type);
	int getID();

	void save(const char* path) override;
	void loadSource(const char* path) override;
	void loadContents(const char* code) override;
private:
	int m_type;
	int m_id;

	void check_shader_compilation(int& shader_id);

};

#endif //AETNA_OPENGLSHADER_H