#include "InputGLFW.h"
#include "../Controls.h"

InputGLFW::InputGLFW() : m_main_window(nullptr) {
}

InputGLFW::InputGLFW(GLFWwindow* window) {
    setMainWindow(window);
}

InputGLFW::~InputGLFW() {
}

void InputGLFW::update() {
    Controls::get()->setKeyState(Controls::Keys::KEY_0, Controls::States::KEY_RELEASE);
    Controls::get()->setKeyState(Controls::Keys::KEY_1, Controls::States::KEY_RELEASE);
    Controls::get()->setKeyState(Controls::Keys::KEY_2, Controls::States::KEY_RELEASE);
    Controls::get()->setKeyState(Controls::Keys::KEY_3, Controls::States::KEY_RELEASE);
    Controls::get()->setKeyState(Controls::Keys::KEY_4, Controls::States::KEY_RELEASE);
    Controls::get()->setKeyState(Controls::Keys::KEY_5, Controls::States::KEY_RELEASE);
    Controls::get()->setKeyState(Controls::Keys::KEY_6, Controls::States::KEY_RELEASE);
    Controls::get()->setKeyState(Controls::Keys::KEY_7, Controls::States::KEY_RELEASE);
    Controls::get()->setKeyState(Controls::Keys::KEY_8, Controls::States::KEY_RELEASE);
    Controls::get()->setKeyState(Controls::Keys::KEY_9, Controls::States::KEY_RELEASE);
    Controls::get()->setKeyState(Controls::Keys::KEY_A, Controls::States::KEY_RELEASE);
    Controls::get()->setKeyState(Controls::Keys::KEY_B, Controls::States::KEY_RELEASE);
    Controls::get()->setKeyState(Controls::Keys::KEY_C, Controls::States::KEY_RELEASE);
    Controls::get()->setKeyState(Controls::Keys::KEY_D, Controls::States::KEY_RELEASE);
    Controls::get()->setKeyState(Controls::Keys::KEY_E, Controls::States::KEY_RELEASE);
    Controls::get()->setKeyState(Controls::Keys::KEY_F, Controls::States::KEY_RELEASE);
    Controls::get()->setKeyState(Controls::Keys::KEY_G, Controls::States::KEY_RELEASE);
    Controls::get()->setKeyState(Controls::Keys::KEY_H, Controls::States::KEY_RELEASE);
    Controls::get()->setKeyState(Controls::Keys::KEY_I, Controls::States::KEY_RELEASE);
    Controls::get()->setKeyState(Controls::Keys::KEY_J, Controls::States::KEY_RELEASE);
    Controls::get()->setKeyState(Controls::Keys::KEY_K, Controls::States::KEY_RELEASE);
    Controls::get()->setKeyState(Controls::Keys::KEY_L, Controls::States::KEY_RELEASE);
    Controls::get()->setKeyState(Controls::Keys::KEY_M, Controls::States::KEY_RELEASE);
    Controls::get()->setKeyState(Controls::Keys::KEY_N, Controls::States::KEY_RELEASE);
    Controls::get()->setKeyState(Controls::Keys::KEY_O, Controls::States::KEY_RELEASE);
    Controls::get()->setKeyState(Controls::Keys::KEY_P, Controls::States::KEY_RELEASE);
    Controls::get()->setKeyState(Controls::Keys::KEY_Q, Controls::States::KEY_RELEASE);
    Controls::get()->setKeyState(Controls::Keys::KEY_R, Controls::States::KEY_RELEASE);
    Controls::get()->setKeyState(Controls::Keys::KEY_S, Controls::States::KEY_RELEASE);
    Controls::get()->setKeyState(Controls::Keys::KEY_T, Controls::States::KEY_RELEASE);
    Controls::get()->setKeyState(Controls::Keys::KEY_U, Controls::States::KEY_RELEASE);
    Controls::get()->setKeyState(Controls::Keys::KEY_V, Controls::States::KEY_RELEASE);
    Controls::get()->setKeyState(Controls::Keys::KEY_W, Controls::States::KEY_RELEASE);
    Controls::get()->setKeyState(Controls::Keys::KEY_X, Controls::States::KEY_RELEASE);
    Controls::get()->setKeyState(Controls::Keys::KEY_Y, Controls::States::KEY_RELEASE);
    Controls::get()->setKeyState(Controls::Keys::KEY_Z, Controls::States::KEY_RELEASE);

    if (glfwGetKey(m_main_window, GLFW_KEY_0) == GLFW_PRESS) {
        Controls::get()->setKeyState(Controls::Keys::KEY_0, Controls::States::KEY_PRESS);
    }
    if (glfwGetKey(m_main_window, GLFW_KEY_1) == GLFW_PRESS) {
        Controls::get()->setKeyState(Controls::Keys::KEY_1, Controls::States::KEY_PRESS);
    }
    if (glfwGetKey(m_main_window, GLFW_KEY_2) == GLFW_PRESS) {
        Controls::get()->setKeyState(Controls::Keys::KEY_2, Controls::States::KEY_PRESS);
    }
    if (glfwGetKey(m_main_window, GLFW_KEY_3) == GLFW_PRESS) {
        Controls::get()->setKeyState(Controls::Keys::KEY_3, Controls::States::KEY_PRESS);
    }
    if (glfwGetKey(m_main_window, GLFW_KEY_4) == GLFW_PRESS) {
        Controls::get()->setKeyState(Controls::Keys::KEY_4, Controls::States::KEY_PRESS);
    }
    if (glfwGetKey(m_main_window, GLFW_KEY_5) == GLFW_PRESS) {
        Controls::get()->setKeyState(Controls::Keys::KEY_5, Controls::States::KEY_PRESS);
    }
    if (glfwGetKey(m_main_window, GLFW_KEY_6) == GLFW_PRESS) {
        Controls::get()->setKeyState(Controls::Keys::KEY_6, Controls::States::KEY_PRESS);
    }
    if (glfwGetKey(m_main_window, GLFW_KEY_7) == GLFW_PRESS) {
        Controls::get()->setKeyState(Controls::Keys::KEY_7, Controls::States::KEY_PRESS);
    }
    if (glfwGetKey(m_main_window, GLFW_KEY_8) == GLFW_PRESS) {
        Controls::get()->setKeyState(Controls::Keys::KEY_8, Controls::States::KEY_PRESS);
    }
    if (glfwGetKey(m_main_window, GLFW_KEY_9) == GLFW_PRESS) {
        Controls::get()->setKeyState(Controls::Keys::KEY_9, Controls::States::KEY_PRESS);
    }
    if (glfwGetKey(m_main_window, GLFW_KEY_A) == GLFW_PRESS) {
        Controls::get()->setKeyState(Controls::Keys::KEY_A, Controls::States::KEY_PRESS);
    }
    if (glfwGetKey(m_main_window, GLFW_KEY_B) == GLFW_PRESS) {
        Controls::get()->setKeyState(Controls::Keys::KEY_B, Controls::States::KEY_PRESS);
    }
    if (glfwGetKey(m_main_window, GLFW_KEY_C) == GLFW_PRESS) {
        Controls::get()->setKeyState(Controls::Keys::KEY_C, Controls::States::KEY_PRESS);
    }
    if (glfwGetKey(m_main_window, GLFW_KEY_D) == GLFW_PRESS) {
        Controls::get()->setKeyState(Controls::Keys::KEY_D, Controls::States::KEY_PRESS);
    }
    if (glfwGetKey(m_main_window, GLFW_KEY_E) == GLFW_PRESS) {
        Controls::get()->setKeyState(Controls::Keys::KEY_E, Controls::States::KEY_PRESS);
    }
    if (glfwGetKey(m_main_window, GLFW_KEY_F) == GLFW_PRESS) {
        Controls::get()->setKeyState(Controls::Keys::KEY_F, Controls::States::KEY_PRESS);
    }
    if (glfwGetKey(m_main_window, GLFW_KEY_G) == GLFW_PRESS) {
        Controls::get()->setKeyState(Controls::Keys::KEY_G, Controls::States::KEY_PRESS);
    }
    if (glfwGetKey(m_main_window, GLFW_KEY_H) == GLFW_PRESS) {
        Controls::get()->setKeyState(Controls::Keys::KEY_H, Controls::States::KEY_PRESS);
    }
    if (glfwGetKey(m_main_window, GLFW_KEY_I) == GLFW_PRESS) {
        Controls::get()->setKeyState(Controls::Keys::KEY_I, Controls::States::KEY_PRESS);
    }
    if (glfwGetKey(m_main_window, GLFW_KEY_J) == GLFW_PRESS) {
        Controls::get()->setKeyState(Controls::Keys::KEY_J, Controls::States::KEY_PRESS);
    }
    if (glfwGetKey(m_main_window, GLFW_KEY_K) == GLFW_PRESS) {
        Controls::get()->setKeyState(Controls::Keys::KEY_K, Controls::States::KEY_PRESS);
    }
    if (glfwGetKey(m_main_window, GLFW_KEY_L) == GLFW_PRESS) {
        Controls::get()->setKeyState(Controls::Keys::KEY_L, Controls::States::KEY_PRESS);
    }
    if (glfwGetKey(m_main_window, GLFW_KEY_M) == GLFW_PRESS) {
        Controls::get()->setKeyState(Controls::Keys::KEY_M, Controls::States::KEY_PRESS);
    }
    if (glfwGetKey(m_main_window, GLFW_KEY_N) == GLFW_PRESS) {
        Controls::get()->setKeyState(Controls::Keys::KEY_N, Controls::States::KEY_PRESS);
    }
    if (glfwGetKey(m_main_window, GLFW_KEY_O) == GLFW_PRESS) {
        Controls::get()->setKeyState(Controls::Keys::KEY_O, Controls::States::KEY_PRESS);
    }
    if (glfwGetKey(m_main_window, GLFW_KEY_P) == GLFW_PRESS) {
        Controls::get()->setKeyState(Controls::Keys::KEY_P, Controls::States::KEY_PRESS);
    }
    if (glfwGetKey(m_main_window, GLFW_KEY_Q) == GLFW_PRESS) {
        Controls::get()->setKeyState(Controls::Keys::KEY_Q, Controls::States::KEY_PRESS);
    }
    if (glfwGetKey(m_main_window, GLFW_KEY_R) == GLFW_PRESS) {
        Controls::get()->setKeyState(Controls::Keys::KEY_R, Controls::States::KEY_PRESS);
    }
    if (glfwGetKey(m_main_window, GLFW_KEY_S) == GLFW_PRESS) {
        Controls::get()->setKeyState(Controls::Keys::KEY_S, Controls::States::KEY_PRESS);
    }
    if (glfwGetKey(m_main_window, GLFW_KEY_T) == GLFW_PRESS) {
        Controls::get()->setKeyState(Controls::Keys::KEY_T, Controls::States::KEY_PRESS);
    }
    if (glfwGetKey(m_main_window, GLFW_KEY_U) == GLFW_PRESS) {
        Controls::get()->setKeyState(Controls::Keys::KEY_U, Controls::States::KEY_PRESS);
    }
    if (glfwGetKey(m_main_window, GLFW_KEY_V) == GLFW_PRESS) {
        Controls::get()->setKeyState(Controls::Keys::KEY_V, Controls::States::KEY_PRESS);
    }
    if (glfwGetKey(m_main_window, GLFW_KEY_W) == GLFW_PRESS) {
        Controls::get()->setKeyState(Controls::Keys::KEY_W, Controls::States::KEY_PRESS);
    }
    if (glfwGetKey(m_main_window, GLFW_KEY_X) == GLFW_PRESS) {
        Controls::get()->setKeyState(Controls::Keys::KEY_X, Controls::States::KEY_PRESS);
    }
    if (glfwGetKey(m_main_window, GLFW_KEY_Y) == GLFW_PRESS) {
        Controls::get()->setKeyState(Controls::Keys::KEY_Y, Controls::States::KEY_PRESS);
    }
    if (glfwGetKey(m_main_window, GLFW_KEY_Z) == GLFW_PRESS) {
        Controls::get()->setKeyState(Controls::Keys::KEY_Z, Controls::States::KEY_PRESS);
    }
}

void InputGLFW::setMainWindow(GLFWwindow* window) {
    m_main_window = window;
}
