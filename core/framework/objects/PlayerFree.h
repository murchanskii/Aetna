#ifndef AETNA_PLAYER_FREE_H
#define AETNA_PLAYER_FREE_H

#include "Player.h"

class PlayerFree : public Player {
public:
	PlayerFree();
	virtual ~PlayerFree();

	const char* getTypeName() override;
};

#endif //AETNA_PLAYER_FREE_H