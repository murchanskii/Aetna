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
	
	virtual void initialize_gbuffer() = 0;
	virtual void initialize_deferred_lighting() = 0;
	virtual void initialize_skybox() = 0;
	virtual void initialize_composition_buffer() = 0;

	virtual void calculate_geometry() = 0;
	virtual void geometry_pass() = 0;
	virtual void lighting_pass() = 0;
	//virtual void depth_pass() = 0;
	virtual void render_lights() = 0;
	virtual void render_skybox() = 0;
	virtual void compose_passes() = 0;
	virtual void postprocess() = 0;

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
