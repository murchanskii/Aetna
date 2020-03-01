#include "InputGLFW.h"
#include "../Engine.h"

std::map<int, Controls::Keys> InputGLFW::keymap;
std::map<int, Controls::MouseButtons> InputGLFW::mousebtnmap;
InputGLFW* InputGLFW::last_instance;

InputGLFW::InputGLFW() : m_main_window(nullptr), 
        key_release_late_update(0), 
        mousebtn_release_late_update(0), 
        scroll_reset_late_update(0) {
    last_instance = this;
}

InputGLFW::InputGLFW(GLFWwindow* window) : 
        key_release_late_update(0), 
        mousebtn_release_late_update(0), 
        scroll_reset_late_update(0) {
    last_instance = this;
    initialize_keymap();
    initialize_mousebtnmap();
    setMainWindow(window);
    glfwSetKeyCallback(window, key_callback); 
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);
}

InputGLFW::~InputGLFW() {

}

void InputGLFW::updateInput() {
    for (std::pair<int, Controls::Keys> t_key : keymap) {
        if (Controls::get()->getKeyState(t_key.second, Controls::States::KEY_RELEASE)) {
            if (key_release_late_update == 1) {
                Controls::get()->setKeyState(t_key.second, Controls::States::KEY_FREE);
                key_release_late_update = 0;
            }
            else {
                ++key_release_late_update;
            }
        }
    }

    for (std::pair<int, Controls::MouseButtons> t_btn : mousebtnmap) {
        if (Controls::get()->getMouseState(t_btn.second, Controls::States::MOUSE_RELEASE)) {
            if (mousebtn_release_late_update == 1) {
                Controls::get()->setMouseState(t_btn.second, Controls::States::MOUSE_FREE);
                mousebtn_release_late_update = 0;
            } else {
                ++mousebtn_release_late_update;
            }
        }
    }

    if (Controls::get()->getScroll() != 0) {
        if (scroll_reset_late_update == 1) {
            last_instance->set_scroll_direction(0);
            scroll_reset_late_update = 0;
        } else {
            ++scroll_reset_late_update;
        }
    }
}

void InputGLFW::setMainWindow(GLFWwindow* window) {
    m_main_window = window;
}

void InputGLFW::terminate() {
    last_instance = nullptr;
    m_main_window = nullptr;
}

void InputGLFW::initialize_keymap() {
    keymap[GLFW_KEY_0] = Controls::Keys::KEY_0;
    keymap[GLFW_KEY_1] = Controls::Keys::KEY_1;
    keymap[GLFW_KEY_2] = Controls::Keys::KEY_2;
    keymap[GLFW_KEY_3] = Controls::Keys::KEY_3;
    keymap[GLFW_KEY_4] = Controls::Keys::KEY_4;
    keymap[GLFW_KEY_5] = Controls::Keys::KEY_5;
    keymap[GLFW_KEY_6] = Controls::Keys::KEY_6;
    keymap[GLFW_KEY_7] = Controls::Keys::KEY_7;
    keymap[GLFW_KEY_8] = Controls::Keys::KEY_8;
    keymap[GLFW_KEY_9] = Controls::Keys::KEY_9;
    keymap[GLFW_KEY_A] = Controls::Keys::KEY_A;
    keymap[GLFW_KEY_B] = Controls::Keys::KEY_B;
    keymap[GLFW_KEY_C] = Controls::Keys::KEY_C;
    keymap[GLFW_KEY_D] = Controls::Keys::KEY_D;
    keymap[GLFW_KEY_E] = Controls::Keys::KEY_E;
    keymap[GLFW_KEY_F] = Controls::Keys::KEY_F;
    keymap[GLFW_KEY_G] = Controls::Keys::KEY_G;
    keymap[GLFW_KEY_H] = Controls::Keys::KEY_H;
    keymap[GLFW_KEY_I] = Controls::Keys::KEY_I;
    keymap[GLFW_KEY_J] = Controls::Keys::KEY_J;
    keymap[GLFW_KEY_K] = Controls::Keys::KEY_K;
    keymap[GLFW_KEY_L] = Controls::Keys::KEY_L;
    keymap[GLFW_KEY_M] = Controls::Keys::KEY_M;
    keymap[GLFW_KEY_N] = Controls::Keys::KEY_N;
    keymap[GLFW_KEY_O] = Controls::Keys::KEY_O;
    keymap[GLFW_KEY_P] = Controls::Keys::KEY_P;
    keymap[GLFW_KEY_Q] = Controls::Keys::KEY_Q;
    keymap[GLFW_KEY_R] = Controls::Keys::KEY_R;
    keymap[GLFW_KEY_S] = Controls::Keys::KEY_S;
    keymap[GLFW_KEY_T] = Controls::Keys::KEY_T;
    keymap[GLFW_KEY_U] = Controls::Keys::KEY_U;
    keymap[GLFW_KEY_V] = Controls::Keys::KEY_V;
    keymap[GLFW_KEY_W] = Controls::Keys::KEY_W;
    keymap[GLFW_KEY_X] = Controls::Keys::KEY_X;
    keymap[GLFW_KEY_Y] = Controls::Keys::KEY_Y;
    keymap[GLFW_KEY_Z] = Controls::Keys::KEY_Z;
}

void InputGLFW::initialize_mousebtnmap() {
    mousebtnmap[GLFW_MOUSE_BUTTON_RIGHT] = Controls::MouseButtons::MOUSE_RIGHT_BUTTON;
    mousebtnmap[GLFW_MOUSE_BUTTON_LEFT] = Controls::MouseButtons::MOUSE_LEFT_BUTTON;
    mousebtnmap[GLFW_MOUSE_BUTTON_MIDDLE] = Controls::MouseButtons::MOUSE_MIDDLE_BUTTON;
}

void InputGLFW::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Controls::States keystate = Controls::States::KEY_FREE;
    switch (action) {
    case GLFW_RELEASE:
        keystate = Controls::States::KEY_RELEASE;
        break;
    case GLFW_PRESS:
        keystate = Controls::States::KEY_PRESS;
        break;
    case GLFW_REPEAT:
        keystate = Controls::States::KEY_PRESS;
        break;
    }

    Controls::get()->setKeyState(keymap[key], keystate);
}

void InputGLFW::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    Controls::States mousebtnstate = Controls::States::MOUSE_FREE;
    switch (action) {
    case GLFW_RELEASE:
        mousebtnstate = Controls::States::MOUSE_RELEASE;
        break;
    case GLFW_PRESS:
        mousebtnstate = Controls::States::MOUSE_PRESS;
        break;
    case GLFW_REPEAT:
        mousebtnstate = Controls::States::MOUSE_PRESS;
        break;
    }

    Controls::get()->setMouseState(mousebtnmap[button], mousebtnstate);
}

void InputGLFW::cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    last_instance->set_mouse_position(xpos, ypos);
}

void InputGLFW::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    last_instance->set_scroll_direction(yoffset);
}
