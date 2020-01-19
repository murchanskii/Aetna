//
// Created by murchanskii on 27.10.2019.
//

#ifndef AETNA_RENDERER_H
#define AETNA_RENDERER_H

#include <stdexcept>

class Renderer {
public:
    static Renderer *get() { return nullptr; }

    Renderer(Renderer const&) = delete;
    Renderer& operator=(Renderer const&) = delete;

    bool isInitialized() {
        return m_is_initialized;
    }

    virtual bool mainWindowIsOpen() = 0;

    virtual void initialize(int window_w, int window_h, const char *window_t) = 0;
    virtual void update() = 0;
    virtual void render() = 0;
    virtual void terminate() = 0;
    virtual void swap() = 0;

protected:
    Renderer() {}
    virtual ~Renderer()  {}

private:
    bool m_is_initialized = false;
};
//Renderer *Renderer::instance = nullptr;

#endif //AETNA_RENDERER_H
