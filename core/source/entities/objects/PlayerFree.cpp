#include "PlayerFree.h"

#include <framework/input/Controls.h>
#include <framework/Engine.h>

#include <glm/gtc/matrix_transform.hpp>

PlayerFree::PlayerFree() : Player() {

}

PlayerFree::~PlayerFree() {

}

const char* PlayerFree::getTypeName() {
	return "PlayerFree";
}