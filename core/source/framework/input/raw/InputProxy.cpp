#include "InputProxy.h"

#include <framework/input/Controls.h>

void InputProxy::set_mouse_position(int x, int y) {
	Controls::get()->set_mouse_position(x, y);
}

void InputProxy::set_scroll_direction(int direction) {
	Controls::get()->set_scroll_direction(direction);
}
