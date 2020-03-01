//
// Created by Candles on 19.01.2020.
//

#ifndef AETNA_GAME_H
#define AETNA_GAME_H

#include "../Object.h"
#include "../Camera.h"

class Camera;

class Game {
public:
    static Game *get() {
        static Game instance;
        return &instance;
    }

    Game(Game const&) = delete;
    Game& operator=(Game const&) = delete;

    int getNumObjectsInScene();
    void addObjectToScene(Object *obj);
    void removeObjectFromScene(int num);
    void removeObjectFromScene(Object *obj);
    Object *getObjectFromScene(int num);
    int findObjectInScene(const char *name);

    Camera* getCamera();
    void setCamera(Camera* cam);

    void update();
    void render();

private:
    Game();
    virtual ~Game();

    struct GameObject {
        Object *object;
    };

    Camera* m_camera;
    bool default_camera;

    std::vector<GameObject> m_vec_gobjects;
};


#endif //AETNA_GAME_H
