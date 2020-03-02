//
// Created by Candles on 19.01.2020.
//

#ifndef AETNA_GAME_H
#define AETNA_GAME_H

#include "../Entity.h"
#include "../Camera.h"

#include <vector>

class Camera;
class Entity;

class Game {
public:
    static Game *get() {
        static Game instance;
        return &instance;
    }

    Game(Game const&) = delete;
    Game& operator=(Game const&) = delete;

    int getNumEntitiesInScene();
    void addEntityToScene(Entity *ent);
    void removeEntityFromScene(int num);
    void removeEntityFromScene(Entity*ent);
    Entity *getEntityFromScene(int num);
    int findEntityInScene(const char *name);

    Camera* getCamera();
    void setCamera(Camera* cam);

    void update();
    void render();

private:
    Game();
    virtual ~Game();

    struct GameEntity {
        Entity *entity;
        bool is_light = false;
    };

    std::vector<int> m_vec_light_dir_indices;
    std::vector<int> m_vec_light_point_indices;
    std::vector<int> m_vec_light_spot_indices;

    Camera* m_camera;
    bool default_camera;

    std::vector<GameEntity> m_vec_gents;
};


#endif //AETNA_GAME_H
