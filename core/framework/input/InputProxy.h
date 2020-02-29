#ifndef AETNA_INPUT_PROXY_H
#define AETNA_INPUT_PROXY_H


class InputProxy {
public:
    InputProxy() {
    }
    virtual ~InputProxy() {
    }

    virtual void update() = 0;
};


#endif //AETNA_INPUT_PROXY_H
