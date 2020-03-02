//
// Created by Candles on 19.01.2020.
//

#include "MainScript.h"
#include "core/framework/Game.h"

#include <iostream>

void MainScript::initialize() {
    std::vector<float> vertices = {
        -0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f, 
         0.5f,  0.5f, -0.5f, 
         0.5f,  0.5f, -0.5f, 
        -0.5f,  0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 

        -0.5f, -0.5f,  0.5f, 
         0.5f, -0.5f,  0.5f, 
         0.5f,  0.5f,  0.5f, 
         0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f,  0.5f, 
        -0.5f, -0.5f,  0.5f, 

        -0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f,  0.5f, 
        -0.5f,  0.5f,  0.5f, 

         0.5f,  0.5f,  0.5f, 
         0.5f,  0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f,  0.5f, 
         0.5f,  0.5f,  0.5f, 

        -0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f,  0.5f, 
         0.5f, -0.5f,  0.5f, 
        -0.5f, -0.5f,  0.5f, 
        -0.5f, -0.5f, -0.5f, 

        -0.5f,  0.5f, -0.5f, 
         0.5f,  0.5f, -0.5f, 
         0.5f,  0.5f,  0.5f, 
         0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f, -0.5f, 
    };

    cube_mesh = new Mesh(vertices);
	material = new Material();
    cube = new Object(cube_mesh, material);

    Game::get()->addEntityToScene(cube);

    player = new PlayerFree();
    Game::get()->setCamera(player->getCamera());
    Game::get()->addEntityToScene(player);

    player->setPosition(glm::vec3(0.0f, 0.0f, 3.0f));

    light_dir = new LightDirectional();
    light_dir->setDirection(glm::vec3(-0.2f, -1.0f, -0.3f));
    light_dir->setAmbient(glm::vec3(0.4f));
    light_dir->setDiffuse(glm::vec3(0.4f));
    Game::get()->addEntityToScene(light_dir);

    light_point = new LightPoint();
    light_point->setAmbient(glm::vec3(0.4f));
    light_point->setDiffuse(glm::vec3(0.4f));
    Game::get()->addEntityToScene(light_point);
}

void MainScript::update() {
    //std::cout << "mainscript is updated" << std::endl;

    light_point->setPosition(player->getPosition());
}

void MainScript::render() {
    //std::cout << "mainscript is rendered" << std::endl;
}

void MainScript::terminate() {
    delete cube;
    delete cube_mesh;
	delete material;
    delete player;
    delete light_dir;
    delete light_point;
    std::cout << "mainscript was terminated" << std::endl;
}
