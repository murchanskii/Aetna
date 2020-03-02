//
// Created by Candles on 19.01.2020.
//

#include "MainScript.h"
#include "core/framework/Game.h"

#include <iostream>

void MainScript::initialize() {
    std::vector<float> vertices = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f,  0.5f, 0.0f
    };

    cube_mesh = new Mesh(vertices);
	material = new Material();
    cube = new Object(cube_mesh, material);

    Game::get()->addEntityToScene(cube);

    player = new PlayerFree();
    Game::get()->setCamera(player->getCamera());
    Game::get()->addEntityToScene(player);

    player->setPosition(glm::vec3(0.0f, 0.0f, 3.0f));
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
    delete player;
    std::cout << "mainscript was terminated" << std::endl;
}
