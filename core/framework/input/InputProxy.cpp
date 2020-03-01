#include "InputProxy.h"

#include "../Controls.h"

void InputProxy::terminate() {
}

void InputProxy::set_mouse_position(int x, int y) {
	Controls::get()->set_mouse_position(x, y);
}

void InputProxy::set_scroll_direction(int direction) {
	Controls::get()->set_scroll_direction(direction);
}
