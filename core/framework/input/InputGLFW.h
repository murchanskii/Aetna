#ifndef AETNA_INPUT_GLFW_H
#define AETNA_INPUT_GLFW_H

#include <GLFW/glfw3.h>

#include "InputProxy.h"
#include "../Controls.h"

#include <map>

class InputGLFW : public InputProxy {
public:
    InputGLFW();
    explicit InputGLFW(GLFWwindow* window);
    virtual ~InputGLFW();

    void updateInput() override;
    void setMainWindow(GLFWwindow *window);
        
private:
    static InputGLFW* last_instance;
    GLFWwindow* m_main_window;

    void initialize_keymap();
    void initialize_mousebtnmap();

    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    
    static std::map<int, Controls::Keys> keymap;
    static std::map<int, Controls::MouseButtons> mousebtnmap;
    int key_release_late_update;
    int mousebtn_release_late_update;
    int scroll_reset_late_update;
};


#endif //AETNA_INPUT_GLFW_H
