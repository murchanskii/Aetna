//
// Created by Candles on 19.01.2020.
//

#ifndef AETNA_MAINSCRIPT_H
#define AETNA_MAINSCRIPT_H

#include "core/framework/Script.h"

class MainScript : public Script {
public:
    void initialize() override;
    void update() override;
    void render() override;
    void terminate() override;

};


#endif //AETNA_MAINSCRIPT_H
