//
// Created by Candles on 19.01.2020.
//

#ifndef AETNA_SCRIPT_H
#define AETNA_SCRIPT_H


class Script {
public:
    Script() {}
    virtual ~Script() {}

    virtual void initialize() = 0;
    virtual void update() = 0;
    virtual void render() = 0;
    virtual void terminate() = 0;
};


#endif //AETNA_SCRIPT_H
