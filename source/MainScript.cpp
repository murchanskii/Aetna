#include "MainScript.h"
#include <framework/Game.h>
#include <framework/Engine.h>
#include <framework/input/Controls.h>

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <iostream>

void MainScript::initialize() {
    Game::get()->loadScene((std::string(Engine::get()->getAssetsPath()) + "project.asc").c_str());
    player = dynamic_cast<PlayerFree*>(Game::get()->getEntityFromScene(Game::get()->findEntityInSceneByName("player")));
    Game::get()->setCamera(player->getCamera());

    cube = dynamic_cast<Object*>(Game::get()->getEntityFromScene(Game::get()->findEntityInSceneByName("cube")));
}

void MainScript::update() {
    Application::get()->setWindowTitle(std::string(
        std::string(AETNA_PROJECT_NAME) + std::string(" | ") +
        std::string("Time: ") + Utils::floatToString(Application::get()->getTime()) + std::string(" | ") + 
        std::string("FPS: ") + Utils::intToString(Application::get()->getFPS())
    ).c_str());
    process_input();

    float speed = Application::get()->getTime() * 100.0f;
    cube->setRotation(glm::quat(cos(glm::radians(speed / 2)), 0, sin(glm::radians(speed / 2)) * 1.0f, 0));
}

void MainScript::render() {

}

void MainScript::terminate() {

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

    float speed = 2.5f * Application::get()->getDeltaTime();
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
