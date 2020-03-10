//
// Created by murchanskii on 27.10.2019.
//

#ifndef AETNA_RENDERER_H
#define AETNA_RENDERER_H

#include "../Game.h"
#include "../../Object.h"
#include "../Engine.h"

#include <iostream>
#include <filesystem>
#include <map>

class Renderer {
protected:
	std::map<std::string, Material*> m_map_ref_materials;
public:
    Renderer(Renderer const&) = delete;
    Renderer& operator=(Renderer const&) = delete;

    bool isInitialized() {
        return m_is_initialized;
    }

    virtual bool mainWindowIsOpen() = 0;
    virtual void setVerticalSync(int value) = 0;

	virtual void initialize(int window_w, int window_h, const char* window_t) = 0;
    virtual void update() = 0;
    virtual void render() = 0;
    virtual void terminate() = 0;
    virtual void swap() = 0;

	Material* getMaterial(const char* name) {
		return m_map_ref_materials[name];
	}

protected:
    friend void Game::addEntityToScene(Entity *ent);
    friend void Game::removeEntityFromScene(Entity *ent);
    friend void Game::removeEntityFromScene(int num);
    friend void Game::render();

    friend class Engine;

    Renderer() {}
    virtual ~Renderer()  {}
    
    virtual void addObjectToRender(Object* obj) = 0;
    virtual void removeObjectFromRender(Object* obj) = 0;
    virtual void removeObjectFromRender(int id) = 0;
    virtual void renderObjects() = 0;

    virtual void resizeWindow(int width, int height) = 0;
    virtual int getWindowHeight() = 0;
    virtual int getWindowWidth() = 0;

	void init_materials(const char* path_to_materials) {
		for (const std::filesystem::directory_entry& entry : std::filesystem::recursive_directory_iterator(path_to_materials)) {
			if (entry.is_regular_file() && entry.path().extension() == ".mat") {
				Material* mat = new Material(entry.path().string().c_str());
				if (!mat->getShaderProgram()) {
					delete mat;
					continue;
				}

				if (m_map_ref_materials[mat->getName().c_str()]) {
					for (int i = 0; ; ++i) {
						std::string new_name = mat->getName() + std::to_string(i);
						if (!m_map_ref_materials[new_name.c_str()]) {
							mat->setName(new_name);
						}
					}
				}

				m_map_ref_materials[mat->getName()] = mat;
			}
		}
	}

private:
    bool m_is_initialized = false;
};

#endif //AETNA_RENDERER_H
