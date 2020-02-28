#ifndef AETNA_CONTROLS_H
#define AETNA_CONTROLS_H

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

    enum class States {
        KEY_RELEASE = 0,
        KEY_PRESS
    };

    bool getKeyState(Keys key, States state);
    void setKeyState(Keys key, States state);

private:
    Controls();
    virtual ~Controls();

    States key_states[35];
};

#endif //AETNA_CONTROLS_H