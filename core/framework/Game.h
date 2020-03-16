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

    void saveScene(const char *path);
	void loadScene(const char* path);

    int getNumEntitiesInScene();
    void addEntityToScene(Entity *ent);
    void removeEntityFromScene(int num);
    void removeEntityFromScene(Entity*ent);
    Entity *getEntityFromScene(int num);
    int findEntityInSceneByName(const char *name);
    int findEntityInSceneById(int id);
    int findEntityInScene(Entity *entity);

    Camera* getCamera();
    void setCamera(Camera* cam);

    void update();
    void render();

private:
    Game();
    virtual ~Game();

    struct GameEntity {
        Entity* entity = nullptr;
        bool is_light = false;
        bool is_manual = false;
    };

    std::vector<int> m_vec_light_dir_indices;
    std::vector<int> m_vec_light_point_indices;
    std::vector<int> m_vec_light_spot_indices;

    Camera* m_camera;
    bool default_camera;

    std::vector<GameEntity> m_vec_gents;
};


#endif //AETNA_GAME_H
