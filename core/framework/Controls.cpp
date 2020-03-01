#include "Controls.h"

Controls::Controls() : mouse_x(0), mouse_y(0), scroll_direction(0) {
	for (int i = 0; i < sizeof(key_states) / sizeof(key_states[0]); ++i) {
		setKeyState((Keys)i, States::KEY_FREE);
	}

	for (int i = 0; i < sizeof(mouse_btn_states) / sizeof(mouse_btn_states[0]); ++i) {
		setMouseState((MouseButtons)i, States::MOUSE_FREE);
	}
}

Controls::~Controls() {

}

void Controls::set_mouse_position(int x, int y) {
	mouse_x = x;
	mouse_y = y;
}

void Controls::set_scroll_direction(int dir) {
	scroll_direction = dir;
}

bool Controls::getKeyState(Keys key, States state) {
	return key_states[(int)key] == state;
}

void Controls::setKeyState(Keys key, States state) {
	key_states[(int)key] = state;
}

bool Controls::getMouseState(MouseButtons mouse_btn, States state) {
	return mouse_btn_states[(int)mouse_btn] == state;
}

void Controls::setMouseState(MouseButtons mouse_btn, States state) {
	mouse_btn_states[(int)mouse_btn] = state;
}

int Controls::getMouseX() {
	return mouse_x;
}

int Controls::getMouseY() {
	return mouse_y;
}

int Controls::getScroll() {
	return scroll_direction;
}
