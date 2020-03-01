#include "PlayerFree.h"

#include "../Controls.h"
#include "../Engine.h"

#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

PlayerFree::PlayerFree() : Player() {

}

PlayerFree::~PlayerFree() {

}

void PlayerFree::setTransform(glm::mat4 t_transform) {
	Object::setTransform(t_transform);
	m_camera->setPosition(glm::vec3(t_transform[3].x, t_transform[3].y, t_transform[3].z));
}

void PlayerFree::setPosition(glm::vec3 pos) {
	Object::setPosition(pos);
	m_camera->setPosition(pos);
}

void PlayerFree::update() {
	Object::update();
	
	float speed = 2.5f * Engine::get()->getFrameTime();
	glm::vec3 pos = getPosition();
	if (Controls::get()->getKeyState(Controls::Keys::KEY_W, Controls::States::KEY_PRESS)) {
		pos += speed * glm::vec3(0.0f, 0.0f, 1.0f);
		setPosition(pos);
	}
	if (Controls::get()->getKeyState(Controls::Keys::KEY_S, Controls::States::KEY_PRESS)) {
		pos -= speed * glm::vec3(0.0f, 0.0f, 1.0f);
	}
	if (Controls::get()->getKeyState(Controls::Keys::KEY_A, Controls::States::KEY_PRESS)) {
		pos -= speed * glm::normalize(glm::cross(glm::vec3(0.0f, 0.0f, 1.0f), m_camera->getUp()));
	}
	if (Controls::get()->getKeyState(Controls::Keys::KEY_D, Controls::States::KEY_PRESS)) {
		pos += speed * glm::normalize(glm::cross(glm::vec3(0.0f, 0.0f, 1.0f), m_camera->getUp()));
	}

	setPosition(pos);
}
