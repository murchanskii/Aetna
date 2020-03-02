//
// Created by Candles on 19.01.2020.
//

#include "MainScript.h"
#include "core/framework/Game.h"
#include "core/framework/Engine.h"

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

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
    cube->setScale(glm::vec3(0.5f));
    cube->setPosition(glm::vec3(1.0f, 0.0f, 0.0f));

    Game::get()->addEntityToScene(cube);

    player = new PlayerFree();
    Game::get()->setCamera(player->getCamera());
    Game::get()->addEntityToScene(player);

    player->setPosition(glm::vec3(0.0f, 0.0f, 3.0f));

    light_dir = new LightDirectional();
    light_dir->setDirection(glm::vec3(-0.2f, -1.0f, -0.3f));
    Game::get()->addEntityToScene(light_dir);

    light_point = new LightPoint();
    light_point->setAmbient(glm::vec3(0.4f));
    light_point->setDiffuse(glm::vec3(0.4f));
    Game::get()->addEntityToScene(light_point);
}

void MainScript::update() {
    //std::cout << "mainscript is updated" << std::endl;

    float angle = Engine::get()->getTime() * 100.0f;
    cube->setRotation(glm::quat(cos(glm::radians(angle / 2)), 0, sin(glm::radians(angle / 2)) * 1.0f, 0));
    cube->setRotation(cube->getRotation());

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
