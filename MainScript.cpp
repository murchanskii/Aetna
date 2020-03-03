//
// Created by Candles on 19.01.2020.
//

#include "MainScript.h"
#include "core/framework/Game.h"
#include "core/framework/Engine.h"
#include "core/framework/Controls.h"

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
    std::vector<float> normals = {
         0.0f,  0.0f, -1.0f,
         0.0f,  0.0f, -1.0f,
         0.0f,  0.0f, -1.0f,
         0.0f,  0.0f, -1.0f,
         0.0f,  0.0f, -1.0f,
         0.0f,  0.0f, -1.0f,

         0.0f,  0.0f,  1.0f,
         0.0f,  0.0f,  1.0f,
         0.0f,  0.0f,  1.0f,
         0.0f,  0.0f,  1.0f,
         0.0f,  0.0f,  1.0f,
         0.0f,  0.0f,  1.0f,

         -1.0f,  0.0f,  0.0f,
         -1.0f,  0.0f,  0.0f,
         -1.0f,  0.0f,  0.0f,
         -1.0f,  0.0f,  0.0f,
         -1.0f,  0.0f,  0.0f,
         -1.0f,  0.0f,  0.0f,

         1.0f,  0.0f,  0.0f,
         1.0f,  0.0f,  0.0f,
         1.0f,  0.0f,  0.0f,
         1.0f,  0.0f,  0.0f,
         1.0f,  0.0f,  0.0f,
         1.0f,  0.0f,  0.0f,

         0.0f, -1.0f,  0.0f,
         0.0f, -1.0f,  0.0f,
         0.0f, -1.0f,  0.0f,
         0.0f, -1.0f,  0.0f,
         0.0f, -1.0f,  0.0f,
         0.0f, -1.0f,  0.0f,

         0.0f,  1.0f,  0.0f,
         0.0f,  1.0f,  0.0f,
         0.0f,  1.0f,  0.0f,
         0.0f,  1.0f,  0.0f,
         0.0f,  1.0f,  0.0f,
         0.0f,  1.0f,  0.0f
    };

    cube_mesh = new Mesh(vertices);
    cube_mesh->setNormals(normals);
	material = new Material();
    cube = new Object(cube_mesh, material);
    cube->setScale(glm::vec3(0.5f));
    cube->setPosition(glm::vec3(1.0f, 0.0f, 0.0f));
    Game::get()->addEntityToScene(cube);

    material2 = new Material();
    material2->getShaderProgram()->setVariable("object_color", &VariableVec4(glm::vec4(1.0f)));
    cube2 = new Object(cube_mesh, material2);
    Game::get()->addEntityToScene(cube2);

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
    process_input();

    float angle = Engine::get()->getTime() * 100.0f;
    cube->setRotation(glm::quat(cos(glm::radians(angle / 2)), 0, sin(glm::radians(angle / 2)) * 1.0f, 0));
    cube2->setRotation(player->getRotation());
    light_point->setPosition(player->getPosition());
}

void MainScript::render() {
    //std::cout << "mainscript is rendered" << std::endl;
}

void MainScript::terminate() {
    delete cube;
    delete cube2;
    delete cube_mesh;
	delete material;
    delete material2;
    delete player;
    delete light_dir;
    delete light_point;
    std::cout << "mainscript was terminated" << std::endl;
}

void MainScript::process_input() {
    glm::vec3 view_dir = player->getCamera()->getDirection();
    static int last_mouse_x = Controls::get()->getMouseX();
    static int last_mouse_y = Controls::get()->getMouseY();

    if (Controls::get()->getMouseState(Controls::MouseButtons::MOUSE_RIGHT_BUTTON, Controls::States::MOUSE_PRESS)) {
        int cur_mouse_x = Controls::get()->getMouseX();
        int cur_mouse_y = Controls::get()->getMouseY();

        float xoffset = cur_mouse_x - last_mouse_x;
        float yoffset = last_mouse_y - cur_mouse_y;
        last_mouse_x = cur_mouse_x;
        last_mouse_y = cur_mouse_y;

        float sensitivity = 0.2f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        static float yaw = -90.0f;
        static float pitch = 0.0f;

        yaw += xoffset;
        pitch += yoffset;

        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        view_dir = glm::normalize(direction);

        player->getCamera()->setDirection(view_dir);
    } else {
        int cur_mouse_x = Controls::get()->getMouseX();
        int cur_mouse_y = Controls::get()->getMouseY();
        last_mouse_x = cur_mouse_x;
        last_mouse_y = cur_mouse_y;
    }

    float speed = 2.5f * Engine::get()->getFrameTime();
    glm::vec3 pos = player->getPosition();
    if (Controls::get()->getKeyState(Controls::Keys::KEY_W, Controls::States::KEY_PRESS)) {
        pos += speed * view_dir;
        player->setPosition(pos);
    }
    if (Controls::get()->getKeyState(Controls::Keys::KEY_S, Controls::States::KEY_PRESS)) {
        pos -= speed * view_dir;
    }
    if (Controls::get()->getKeyState(Controls::Keys::KEY_A, Controls::States::KEY_PRESS)) {
        pos -= speed * glm::normalize(glm::cross(view_dir, player->getCamera()->getUp()));
    }
    if (Controls::get()->getKeyState(Controls::Keys::KEY_D, Controls::States::KEY_PRESS)) {
        pos += speed * glm::normalize(glm::cross(view_dir, player->getCamera()->getUp()));
    }
    if (Controls::get()->getKeyState(Controls::Keys::KEY_Q, Controls::States::KEY_PRESS)) {
        pos -= speed * player->getCamera()->getUp();
    }
    if (Controls::get()->getKeyState(Controls::Keys::KEY_E, Controls::States::KEY_PRESS)) {
        pos += speed * player->getCamera()->getUp();
    }

    player->setPosition(pos);
}
