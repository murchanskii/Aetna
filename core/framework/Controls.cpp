#include "Controls.h"

Controls::Controls() {
	for (int i = 0; i < sizeof(key_states) / sizeof(key_states[0]); ++i) {
		setKeyState((Keys)i, States::KEY_RELEASE);
	}
}

Controls::~Controls() {

}

bool Controls::getKeyState(Keys key, States state) {
	return key_states[(int)key] == state;
}

void Controls::setKeyState(Keys key, States state) {
	key_states[(int)key] = state;
}