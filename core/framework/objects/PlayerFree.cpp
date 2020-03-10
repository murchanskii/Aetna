#include "PlayerFree.h"

#include "../Controls.h"
#include "../Engine.h"

#include <glm/gtc/matrix_transform.hpp>

PlayerFree::PlayerFree() : Player() {

}

PlayerFree::~PlayerFree() {

}

const char* PlayerFree::getTypeName() {
	return "PlayerFree";
}