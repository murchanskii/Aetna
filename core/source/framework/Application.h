#ifndef AETNA_APPLICATION_H
#define AETNA_APPLICATION_H

#include <framework/render/Renderer.h>
#include <framework/input/raw/InputProxy.h>

#include <string>

class Application {
public:
    static Application* get() {
        return instance;
    }

    Renderer* getRenderer();

    Application(Application const&) = delete;
    Application& operator=(Application const&) = delete;

    virtual void initialize(int init_width, int init_height, const char *init_title) = 0;
    virtual void preUpdate() = 0;
    virtual void update() = 0;
    virtual void postUpdate() = 0;
    virtual void swap() = 0;
	virtual void terminate() = 0;

    std::string getDirectory();

    virtual int getWindowWidth() = 0;
    virtual int getWindowHeight() = 0;
    virtual void resizeWindow(int w, int h) = 0;
    virtual const char* getWindowTitle() = 0;
    virtual void setWindowTitle(const char* title) = 0;

    virtual bool windowIsOpen() = 0;

    virtual void setVerticalSync(int value) = 0;

    virtual float getTime() = 0;
    virtual float getDeltaTime() = 0;
    virtual int getFPS() = 0;

protected:
    explicit Application(Application* app);
    virtual ~Application();

    static Application *instance;
    Renderer* m_renderer;
    InputProxy* m_input_proxy;

    std::string m_window_title;
    std::string m_app_path;
};

#endif //AETNA_APPLICATION_H