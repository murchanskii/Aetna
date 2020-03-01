#ifndef AETNA_CONTROLS_H
#define AETNA_CONTROLS_H

#include "input/InputProxy.h"

class Controls {
public:
    static Controls* get() {
        static Controls instance;
        return &instance;
    }

    Controls(Controls const&) = delete;
    Controls& operator=(Controls const&) = delete;

    enum class Keys {
        KEY_0 = 0,
        KEY_1,
        KEY_2,
        KEY_3,
        KEY_4,
        KEY_5,
        KEY_6,
        KEY_7,
        KEY_8,
        KEY_9,
        KEY_A,
        KEY_B,
        KEY_C,
        KEY_D,
        KEY_E,
        KEY_F, 
        KEY_G,
        KEY_H,
        KEY_I,
        KEY_J,
        KEY_K,
        KEY_L,
        KEY_M,
        KEY_N,
        KEY_O,
        KEY_P,
        KEY_Q,
        KEY_R,
        KEY_S,
        KEY_T,
        KEY_U, 
        KEY_V,
        KEY_W,
        KEY_X,
        KEY_Y,
        KEY_Z
    };

    enum class MouseButtons {
        MOUSE_RIGHT_BUTTON = 0,
        MOUSE_MIDDLE_BUTTON,
        MOUSE_LEFT_BUTTON
    };

    enum class States {
        KEY_FREE = 0,
        KEY_RELEASE,
        KEY_PRESS,

        MOUSE_FREE,
        MOUSE_RELEASE,
        MOUSE_PRESS
    };

    bool getKeyState(Keys key, States state);
    void setKeyState(Keys key, States state);

    bool getMouseState(MouseButtons mouse_btn, States state);
    void setMouseState(MouseButtons mouse_btn, States state);

    int getMouseX();
    int getMouseY();

    int getScroll();

private:
    Controls();
    virtual ~Controls();
    friend class InputProxy;

    States key_states[35];
    States mouse_btn_states[3];
    int mouse_x;
    int mouse_y;
    int scroll_direction;

    void set_mouse_position(int x, int y);
    void set_scroll_direction(int dir);
};

#endif //AETNA_CONTROLS_H