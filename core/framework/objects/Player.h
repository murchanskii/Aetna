#ifndef AETNA_PLAYER_H
#define AETNA_PLAYER_H

#include "../../Object.h"
#include "../../Camera.h"

class Player : public Object {
public:
	virtual Camera* getCamera() {
		return m_camera;
	}

	void setTransform(glm::mat4 t_transform) override {
		Object::setTransform(t_transform);
		m_camera->setPosition(glm::vec3(t_transform[3].x, t_transform[3].y, t_transform[3].z));
	}

	void setPosition(glm::vec3 pos) override {
		Object::setPosition(pos);
		m_camera->setPosition(pos);
	}

protected:
	Player() {
		m_camera = new Camera();
	}

	virtual ~Player() {
		if (m_camera) {
			delete m_camera;
		}
	}

	Camera* m_camera = nullptr;
};

#endif //AETNA_PLAYER_H