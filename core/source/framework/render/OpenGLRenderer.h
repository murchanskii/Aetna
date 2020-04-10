#ifndef AETNA_OPENGLRENDERER_H
#define AETNA_OPENGLRENDERER_H

#include <framework/ApplicationGL.h>
#include <framework/render/shader/OpenGLShaderProgram.h>

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
        Object *scene_object;
        unsigned int VAO;
        std::vector<unsigned int> vec_VBOs;
    };

    void initialize_skybox_buffer() override;
    void initialize_shadow_buffer() override;
    void initialize_gbuffer() override;
    void initialize_depth_buffer() override;
    void initialize_deferred_light_buffer() override;
    void initialize_auxiliary_buffer() override;
    void initialize_postmaterials_buffer() override;
    void initialize_gui_buffer() override;

    void render_skybox_buffer() override;
    void calculate_geometry() override;
    void render_shadow_buffer() override;
    void render_gbuffer() override;
    void render_depth_buffer() override;
    void render_deferred_light_buffer() override;
    void render_auxiliary_buffer() override;
    void render_postmaterials_buffer() override;
    void render_gui_buffer() override;

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
