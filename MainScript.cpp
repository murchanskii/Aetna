//
// Created by Candles on 19.01.2020.
//

#include "MainScript.h"
#include <iostream>

void MainScript::initialize() {
    std::cout << "mainscript was initialized" << std::endl;
}

void MainScript::update() {
    std::cout << "mainscript is updated" << std::endl;
}

void MainScript::render() {
    std::cout << "mainscript is rendered" << std::endl;
}

void MainScript::terminate() {
    std::cout << "mainscript was terminated" << std::endl;
}
