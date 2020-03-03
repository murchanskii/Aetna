#include "PlayerFree.h"

#include "../Controls.h"
#include "../Engine.h"

#include <glm/gtc/matrix_transform.hpp>

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
	
	glm::vec3 view_dir = m_camera->getDirection();
	static int last_mouse_x = Controls::get()->getMouseX();
	static int last_mouse_y = Controls::get()->getMouseY();

	if (Controls::get()->getMouseState(Controls::MouseButtons::MOUSE_RIGHT_BUTTON, Controls::States::MOUSE_PRESS)) {
		int cur_mouse_x = Controls::get()->getMouseX();
		int cur_mouse_y = Controls::get()->getMouseY();

		float xoffset = cur_mouse_x - last_mouse_x;
		float yoffset = last_mouse_y - cur_mouse_y;
		last_mouse_x = cur_mouse_x;
		last_mouse_y = cur_mouse_y;

		float sensitivity = 0.2f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		static float yaw = -90.0f;
		static float pitch = 0.0f;

		yaw += xoffset;
		pitch += yoffset;

		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		glm::vec3 direction;
		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		view_dir = glm::normalize(direction);

		m_camera->setDirection(view_dir);
	}
	else {
		int cur_mouse_x = Controls::get()->getMouseX();
		int cur_mouse_y = Controls::get()->getMouseY();
		last_mouse_x = cur_mouse_x;
		last_mouse_y = cur_mouse_y;
	}

	float speed = 2.5f * Engine::get()->getFrameTime();
	glm::vec3 pos = getPosition();
	if (Controls::get()->getKeyState(Controls::Keys::KEY_W, Controls::States::KEY_PRESS)) {
		pos += speed * view_dir;
		setPosition(pos);
	}
	if (Controls::get()->getKeyState(Controls::Keys::KEY_S, Controls::States::KEY_PRESS)) {
		pos -= speed * view_dir;
	}
	if (Controls::get()->getKeyState(Controls::Keys::KEY_A, Controls::States::KEY_PRESS)) {
		pos -= speed * glm::normalize(glm::cross(view_dir, m_camera->getUp()));
	}
	if (Controls::get()->getKeyState(Controls::Keys::KEY_D, Controls::States::KEY_PRESS)) {
		pos += speed * glm::normalize(glm::cross(view_dir, m_camera->getUp()));
	}
	if (Controls::get()->getKeyState(Controls::Keys::KEY_Q, Controls::States::KEY_PRESS)) {
		pos -= speed * m_camera->getUp();
	}
	if (Controls::get()->getKeyState(Controls::Keys::KEY_E, Controls::States::KEY_PRESS)) {
		pos += speed * m_camera->getUp();
	}

	setPosition(pos);
}
