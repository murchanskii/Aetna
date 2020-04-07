#ifndef AETNA_INPUT_PROXY_H
#define AETNA_INPUT_PROXY_H

class InputProxy {
public:
    InputProxy() {

    }
    virtual ~InputProxy() {

    }

    virtual void updateInput() = 0;

protected:
    void set_mouse_position(int x, int y);
    void set_scroll_direction(int direction);
};


#endif //AETNA_INPUT_PROXY_H
