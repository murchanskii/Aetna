//
// Created by Candles on 19.01.2020.
//

#include "MainScript.h"
#include "core/framework/Game.h"
#include <iostream>

#include "core/framework/Engine.h"

void MainScript::initialize() {
    std::vector<float> vertices = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f,  0.5f, 0.0f
    };

    cube_mesh = new Mesh(vertices);
	material = new Material();
    cube = new Object(cube_mesh, material);

    Game::get()->addObjectToScene(cube);
}

void MainScript::update() {
    //std::cout << "mainscript is updated" << std::endl;
}

void MainScript::render() {
    //std::cout << "mainscript is rendered" << std::endl;
}

void MainScript::terminate() {
    delete cube;
    delete cube_mesh;
	delete material;
    std::cout << "mainscript was terminated" << std::endl;
}
