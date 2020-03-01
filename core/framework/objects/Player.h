#ifndef AETNA_PLAYER_H
#define AETNA_PLAYER_H

#include "../../Object.h"
#include "../../Camera.h"

class Player : public Object {
public:
	virtual Camera* getCamera() {
		return m_camera;
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