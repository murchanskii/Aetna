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

    void create_gl_object(OpenGLObject** gl_object, Object *obj);

	void initialize_gbuffer() override;
	void initialize_deferred_lighting() override;
	void initialize_skybox() override;
	void initialize_composition_buffer() override;
	void calculate_geometry() override;
	void geometry_pass() override;
	void lighting_pass() override;
	void depth_pass(unsigned int src_buf, unsigned int dest_buf);
	void render_lights() override;
	void render_skybox() override;
	void compose_passes() override;
	void postprocess() override;
	
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

    Material* m_skybox_mat = nullptr;
    Material* m_gbuffer_mat = nullptr;
    Material* m_deferred_lighting_mat = nullptr;
	Material* m_composition_mat = nullptr;
	
	unsigned int m_mainBuffer;
	unsigned int m_composition_rbo_depth;
	unsigned int m_mainBufferOutputColor;
	
    unsigned int m_gBuffer;
	unsigned int m_gbuffer_rbo_depth;
    unsigned int m_gBufferPosition;
    unsigned int m_gBufferNormal;
    unsigned int m_gBufferAlbedo;
    unsigned int m_gBufferSpecular;
	
    unsigned int m_skybox_cubemap;

	OpenGLObject* m_deferred_light_quad = nullptr;
	OpenGLObject* m_skybox_quad = nullptr;
	OpenGLObject* m_composition_quad = nullptr;
    
    std::vector<OpenGLObject*> m_vec_gl_objects;

    std::vector<int> m_vec_light_dir_indices;
    std::vector<int> m_vec_light_point_indices;
    std::vector<int> m_vec_light_spot_indices;
};


#endif //AETNA_OPENGLRENDERER_H
