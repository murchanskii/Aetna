#ifndef AETNA_CAMERA_H
#define AETNA_CAMERA_H

#include "framework/Game.h"
#include <glm/glm.hpp>

class Camera {
public:
	Camera();
	Camera(glm::vec3 pos, float fov = 45.0f, float znear = 0.01f, float zfar = 100.0f);

	void setZNear(float znear);
	float getZNear();
	void setZFar(float zfar);
	float getZFar();

	void setFOV(float fov);
	float getFOV();

	void setUp(glm::vec3 up);
	glm::vec3 getUp();

	void setPosition(glm::vec3 pos);
	glm::vec3 getPosition();

	void setView(glm::mat4 view);
	glm::mat4 getView();

	void setDirection(glm::vec3 direction);
	glm::vec3 getDirection();

	void setProjection(glm::mat4 projection);
	glm::mat4 getProjection();

private:
	glm::mat4 m_projection;
	glm::mat4 m_view;
	glm::vec3 m_position;

	glm::vec3 m_up;
	glm::vec3 m_direction;
	float m_fov;
	float m_znear;
	float m_zfar;

	friend class Game;

	void update();
};

#endif //AETNA_CAMERA_H