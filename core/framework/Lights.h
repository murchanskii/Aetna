#ifndef AETNA_LIGHTS_H
#define AETNA_LIGHTS_H

#include "../Entity.h"

class Light : public Entity {
protected:
	glm::vec4 m_color;
	glm::vec3 m_ambient;
	glm::vec3 m_diffuse;
	glm::vec3 m_specular;

	Light() : Entity(), 
			m_color(glm::vec4(1.0f)), 
			m_ambient(glm::vec4(0.4f)),
			m_diffuse(glm::vec4(0.4f)),
			m_specular(glm::vec4(0.3f)) {

	}
	virtual ~Light() {

	}
public:
	const char* getTypeName() override {
		return "Light";
	}

	virtual void setColor(glm::vec4 color) {
		m_color = color;
	}
	virtual glm::vec4 getColor() {
		return m_color;
	}

	virtual void setAmbient(glm::vec3 ambient) {
		m_ambient = ambient;
	}
	virtual glm::vec3 getAmbient() {
		return m_ambient;
	}

	virtual void setDiffuse(glm::vec3 diffuse) {
		m_diffuse = diffuse;
	}
	virtual glm::vec3 getDiffuse() {
		return m_diffuse;
	}

	virtual void setSpecular(glm::vec3 specular) {
		m_specular = specular;
	}
	virtual glm::vec3 getSpecular() {
		return m_specular;
	}
};

class LightDirectional : public Light {
protected:
	glm::vec3 m_direction;
public:
	LightDirectional() : Light(), m_direction(glm::vec3(0.0f)) {

	}
	
	const char* getTypeName() override {
		return "LightDirectional";
	}

	virtual void setDirection(glm::vec3 dir) {
		m_direction = dir;
	}

	virtual glm::vec3 getDirection() {
		return m_direction;
	}
};

class LightPoint : public Light {
protected:
	float m_constant;
	float m_linear;
	float m_quadratic;

public:
	LightPoint() : Light(), m_constant(1.0f), m_linear(0.14f), m_quadratic(0.07f) {

	}

	const char* getTypeName() override {
		return "LightPoint";
	}

	void setConstant(float constant) {
		m_constant = constant;
	}
	float getConstant() {
		return m_constant;
	}

	void setLinear(float linear) {
		m_linear = linear;
	}
	float getLinear() {
		return m_linear;
	}

	void setQuadratic(float quadratic) {
		m_quadratic = quadratic;
	}
	float getQuadratic() {
		return m_quadratic;
	}
};

class LightSpot : public Light {
protected:

public:
	LightSpot() : Light() {

	}

	const char* getTypeName() override {
		return "LightSpot";
	}
};

#endif //AETNA_LIGHTS_H