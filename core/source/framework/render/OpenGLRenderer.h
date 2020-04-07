#ifndef AETNA_OPENGLRENDERER_H
#define AETNA_OPENGLRENDERER_H

#include <framework/ApplicationGL.h>

#include <vector>

class OpenGLRenderer : public Renderer {
public:
    OpenGLRenderer();
    ~OpenGLRenderer() override;

    bool isInitialized() override;

    void initialize(void* param = nullptr) override;
    void update() override;
    void render() override;
    void terminate() override;
    

protected:
    struct OpenGLObject {
        struct OpenGLTexture {
            unsigned int location;
            int num;
            std::string name;
        };

        Object *scene_object;
        unsigned int VAO;
        std::vector<unsigned int> vec_VBOs;
        std::vector<OpenGLTexture*> vec_textures;
    };

    void addObjectToRender(Object *obj) override;
    void removeObjectFromRender(Object *obj) override;
    void removeObjectFromRender(int id) override;
    void renderObjects() override;

    friend class ApplicationGL;

    void resizeViewport(int width, int height) override;
    int getViewportWidth() override;
    int getViewportHeight() override;

private:
    bool m_initialized;
    
    std::vector<OpenGLObject*> m_vec_gl_objects;
};


#endif //AETNA_OPENGLRENDERER_H
