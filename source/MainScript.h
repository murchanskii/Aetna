//
// Created by Candles on 19.01.2020.
//

#ifndef AETNA_MAINSCRIPT_H
#define AETNA_MAINSCRIPT_H

#include <framework/Script.h>
#include <Mesh.h>
#include <entities/objects/Object.h>
#include <entities/objects/PlayerFree.h>
#include <entities/Lights.h>

class MainScript : public Script {
public:
    void initialize() override;
    void update() override;
    void render() override;
    void terminate() override;

private:
    Mesh *cube_mesh;
    Object *cube;
	Material* material;

    Object* cube2;
    Material* material2;

    LightDirectional* light_dir;
    LightPoint* light_point;

    PlayerFree* player;

    void process_input();
};


#endif //AETNA_MAINSCRIPT_H
