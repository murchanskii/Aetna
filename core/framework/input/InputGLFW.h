#ifndef AETNA_INPUT_GLFW_H
#define AETNA_INPUT_GLFW_H

#include <GLFW/glfw3.h>
#include "InputProxy.h"

class InputGLFW : public InputProxy {
public:
    InputGLFW();
    explicit InputGLFW(GLFWwindow* window);
    virtual ~InputGLFW();

    void update() override;
    void setMainWindow(GLFWwindow *window);

private:
    GLFWwindow* m_main_window;
};


#endif //AETNA_INPUT_GLFW_H
