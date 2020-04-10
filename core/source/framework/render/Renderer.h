#ifndef AETNA_RENDERER_H
#define AETNA_RENDERER_H

#include <framework/Game.h>
#include <entities/objects/Object.h>

#include <iostream>

class Renderer {
public:
    virtual ~Renderer() { }

    virtual bool isInitialized() = 0;

	virtual void initialize(void* param = nullptr) = 0;
    virtual void update() = 0;
    virtual void render() = 0;
    virtual void terminate() = 0;
    
protected:
    Renderer() { }

    virtual void initialize_skybox_buffer() = 0;
    virtual void initialize_shadow_buffer() = 0;
    virtual void initialize_gbuffer() = 0;
    virtual void initialize_depth_buffer() = 0;
    virtual void initialize_deferred_light_buffer() = 0;
    virtual void initialize_auxiliary_buffer() = 0;
    virtual void initialize_postmaterials_buffer() = 0;
    virtual void initialize_gui_buffer() = 0;

    virtual void render_skybox_buffer() = 0;
    virtual void calculate_geometry() = 0;
    virtual void render_shadow_buffer() = 0;
    virtual void render_gbuffer() = 0;
    virtual void render_depth_buffer() = 0;
    virtual void render_deferred_light_buffer() = 0;
    virtual void render_auxiliary_buffer() = 0;
    virtual void render_postmaterials_buffer() = 0;
    virtual void render_gui_buffer() = 0;


    friend class Game;
        
    virtual void addObjectToRender(Object* obj) = 0;
    virtual void removeObjectFromRender(Object* obj) = 0;
    virtual void removeObjectFromRender(int id) = 0;
    virtual void renderObjects() = 0;

    virtual void resizeViewport(int width, int height) = 0;
    virtual int getViewportHeight() = 0;
    virtual int getViewportWidth() = 0;
};

#endif //AETNA_RENDERER_H
