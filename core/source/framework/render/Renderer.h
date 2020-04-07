#ifndef AETNA_RENDERER_H
#define AETNA_RENDERER_H

#include <framework/Game.h>
#include <entities/objects/Object.h>

#include <iostream>

class Renderer {
public:
    virtual ~Renderer() {
    }

    virtual bool isInitialized() = 0;

	virtual void initialize(void* param = nullptr) = 0;
    virtual void update() = 0;
    virtual void render() = 0;
    virtual void terminate() = 0;
    
protected:
    Renderer() { }

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
