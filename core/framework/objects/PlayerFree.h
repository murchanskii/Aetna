#ifndef AETNA_PLAYER_FREE_H
#define AETNA_PLAYER_FREE_H

#include "Player.h"

class PlayerFree : public Player {
public:
	PlayerFree();
	virtual ~PlayerFree();

	void setTransform(glm::mat4 t_transform) override;
	void setPosition(glm::vec3 pos) override;

protected:
	void update() override;
};

#endif //AETNA_PLAYER_FREE_H