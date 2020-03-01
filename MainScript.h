//
// Created by Candles on 19.01.2020.
//

#ifndef AETNA_MAINSCRIPT_H
#define AETNA_MAINSCRIPT_H

#include "core/framework/Script.h"
#include "core/Mesh.h"
#include "core/Object.h"
#include "core/framework/objects/PlayerFree.h"

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

    PlayerFree* player;
};


#endif //AETNA_MAINSCRIPT_H
