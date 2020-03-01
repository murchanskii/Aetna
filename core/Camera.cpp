#include "Camera.h"

#include "framework/Engine.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera() :
		m_fov(45.0f), 
		m_position(glm::vec3(0.0f)),
		m_up(glm::vec3(0.0f, 1.0f, 0.0f)),
		m_znear(0.01f),
		m_zfar(100.0f) {
	update();
}

Camera::Camera(glm::vec3 pos, float fov, float znear, float zfar) : 
		m_up(glm::vec3(0.0f, 1.0f, 0.0f)) {
	m_position = pos;
	update();
}

void Camera::setZNear(float znear) {
	m_znear = znear;
}

float Camera::getZNear() {
	return m_znear;
}

void Camera::setZFar(float zfar) {
	m_zfar = zfar;
}

float Camera::getZFar() {
	return m_zfar;
}

void Camera::setFOV(float fov) {
	m_fov = fov;
}

float Camera::getFOV() {
	return m_fov;
}

void Camera::setUp(glm::vec3 up) {
	m_up = up;
}

glm::vec3 Camera::getUp() {
	return m_up;
}

void Camera::setPosition(glm::vec3 pos) {
	m_position = pos;
}

glm::vec3 Camera::getPosition() {
	return m_position;
}

void Camera::setView(glm::mat4 view) {
	m_view = view;
}

glm::mat4 Camera::getView() {
	return m_view;
}

void Camera::setProjection(glm::mat4 projection) {
	m_projection = projection;
}

glm::mat4 Camera::getProjection() {
	return m_projection;
}

void Camera::update() {
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

	m_view = glm::lookAt(m_position, m_position + cameraFront, m_up);
	m_projection = glm::perspective(glm::radians(m_fov), Engine::get()->getWindowWidth() / (float) Engine::get()->getWindowHeight(), m_znear, m_zfar);
}
