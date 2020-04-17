#include "OpenGLRenderer.h"

#include <glad/glad.h>

#include <framework/Materials.h>
#include <entities/Lights.h>

#include <stb_image.h>

#include <iostream>
#include <algorithm>

OpenGLRenderer::OpenGLRenderer() : m_initialized(false) {

}

OpenGLRenderer::~OpenGLRenderer() {

}

bool OpenGLRenderer::isInitialized() {
    return m_initialized;
}

void OpenGLRenderer::initialize(void *param) {
    if (!gladLoadGLLoader((GLADloadproc)param)) {
        std::cout << "Failed to initialize glad" << std::endl;
        return;
    }
    glViewport(0, 0, Application::get()->getWindowWidth(), Application::get()->getWindowHeight());

	initialize_gbuffer();
	initialize_deferred_lighting();
	initialize_skybox();
	initialize_composition_buffer();

    m_initialized = true;
}

void OpenGLRenderer::update() {

}

void OpenGLRenderer::render() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	calculate_geometry();
	
    glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	geometry_pass();
	
    glBindFramebuffer(GL_FRAMEBUFFER, m_mainBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	lighting_pass();
	depth_pass(m_gBuffer, m_mainBuffer);
	render_lights();		
	render_skybox();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	compose_passes();
}

void OpenGLRenderer::terminate() {
    for (int i = 0; i < m_vec_gl_objects.size(); ++i) {
        removeObjectFromRender(i);
    }
}

void OpenGLRenderer::addObjectToRender(Object *obj) {
    for (int i = 0; i < m_vec_gl_objects.size(); ++i) {
        if (m_vec_gl_objects[i]->scene_object == obj) {
            return;
        }
    }

    OpenGLObject* gl_object = nullptr;
    create_gl_object(&gl_object, obj);
    if (gl_object) {
        m_vec_gl_objects.push_back(gl_object);
    }
}

void OpenGLRenderer::removeObjectFromRender(int id) {
    OpenGLObject *gl_object = m_vec_gl_objects[id];
    m_vec_gl_objects.erase(m_vec_gl_objects.begin() + id);
    delete gl_object;
}

void OpenGLRenderer::removeObjectFromRender(Object *obj) {
    int id_found = -1;
    for (int i = 0; i < m_vec_gl_objects.size(); ++i) {
        if (obj == m_vec_gl_objects[i]->scene_object) {
            id_found = i;
            break;
        }
    }

    if (id_found == -1) {
        return;
    }

    removeObjectFromRender(id_found);
}

void OpenGLRenderer::renderObjects() {
    for (OpenGLObject *gl_object : m_vec_gl_objects) {
        if (gl_object->scene_object->isEnabled() && gl_object->scene_object->getMaterial()) {
            gl_object->scene_object->getMaterial()->use();
            glBindVertexArray(gl_object->VAO);
            glDrawArrays(GL_TRIANGLES, 0, gl_object->scene_object->getMesh()->getVertices().size() / 3);
            glBindVertexArray(0);
        }
    }
}

void OpenGLRenderer::resizeViewport(int width, int height) {
    glViewport(0, 0, width, height);
    glScissor(0, 0, width, height);

	// update main buffer size
    glBindRenderbuffer(GL_RENDERBUFFER, m_composition_rbo_depth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, Application::get()->getWindowWidth(), Application::get()->getWindowHeight());
    glBindTexture(GL_TEXTURE_2D, m_mainBufferOutputColor);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Application::get()->getWindowWidth(), Application::get()->getWindowHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	// update gbuffer size
	glBindRenderbuffer(GL_RENDERBUFFER, m_gbuffer_rbo_depth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, Application::get()->getWindowWidth(), Application::get()->getWindowHeight());
	glBindTexture(GL_TEXTURE_2D, m_gBufferPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, Application::get()->getWindowWidth(), Application::get()->getWindowHeight(), 0, GL_RGB, GL_FLOAT, NULL);
	glBindTexture(GL_TEXTURE_2D, m_gBufferNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, Application::get()->getWindowWidth(), Application::get()->getWindowHeight(), 0, GL_RGB, GL_FLOAT, NULL);
	glBindTexture(GL_TEXTURE_2D, m_gBufferAlbedo);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Application::get()->getWindowWidth(), Application::get()->getWindowHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, m_gBufferSpecular);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Application::get()->getWindowWidth(), Application::get()->getWindowHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	   	 
    Application::get()->getRenderer()->render();
}

int OpenGLRenderer::getViewportWidth() {
    GLint vp[4]; 
    glGetIntegerv(GL_VIEWPORT, vp);
    return vp[2];
}

int OpenGLRenderer::getViewportHeight() {
    GLint vp[4];
    glGetIntegerv(GL_VIEWPORT, vp);
    return vp[3];
}

void OpenGLRenderer::initialize_gbuffer()
{
	glGenFramebuffers(1, &m_gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer);
	unsigned int attachments[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };

	glGenTextures(1, &m_gBufferPosition);
	glBindTexture(GL_TEXTURE_2D, m_gBufferPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, Application::get()->getWindowWidth(), Application::get()->getWindowHeight(), 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachments[0], GL_TEXTURE_2D, m_gBufferPosition, 0);

	glGenTextures(1, &m_gBufferNormal);
	glBindTexture(GL_TEXTURE_2D, m_gBufferNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, Application::get()->getWindowWidth(), Application::get()->getWindowHeight(), 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachments[1], GL_TEXTURE_2D, m_gBufferNormal, 0);

	glGenTextures(1, &m_gBufferAlbedo);
	glBindTexture(GL_TEXTURE_2D, m_gBufferAlbedo);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Application::get()->getWindowWidth(), Application::get()->getWindowHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachments[2], GL_TEXTURE_2D, m_gBufferAlbedo, 0);

	glGenTextures(1, &m_gBufferSpecular);
	glBindTexture(GL_TEXTURE_2D, m_gBufferSpecular);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Application::get()->getWindowWidth(), Application::get()->getWindowHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachments[3], GL_TEXTURE_2D, m_gBufferSpecular, 0);

	glDrawBuffers(4, attachments);

	glGenRenderbuffers(1, &m_gbuffer_rbo_depth);
	glBindRenderbuffer(GL_RENDERBUFFER, m_gbuffer_rbo_depth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, Application::get()->getWindowWidth(), Application::get()->getWindowHeight());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_gbuffer_rbo_depth);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "GBuffer not complete!" << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLRenderer::initialize_deferred_lighting()
{
	std::vector<float> quadVertices = {
		-1.0f,  1.0f,  0.0f,
		-1.0f, -1.0f,  0.0f,
		 1.0f, -1.0f,  0.0f,

		-1.0f,  1.0f,  0.0f,
		 1.0f, -1.0f,  0.0f,
		 1.0f,  1.0f,  0.0f
	};
	std::vector<float> quadTexCoords = {
		 0.0f, 1.0f,
		 0.0f, 0.0f,
		 1.0f, 0.0f,

		 0.0f, 1.0f,
		 1.0f, 0.0f,
		 1.0f, 1.0f
	};
	Mesh* mesh_light_quad = new Mesh(quadVertices);
	mesh_light_quad->setTextureCoordinates(quadTexCoords);
	Object* obj_light_quad = new Object(mesh_light_quad);
	create_gl_object(&m_deferred_light_quad, obj_light_quad);

	m_deferred_lighting_mat = new Material();
	m_deferred_lighting_mat->load(Materials::get()->getMaterialPath("deferred_lighting").c_str());
	m_deferred_lighting_mat->setVariable("gPosition", &VariableInt(0));
	m_deferred_lighting_mat->setVariable("gNormal", &VariableInt(1));
	m_deferred_lighting_mat->setVariable("gAlbedo", &VariableInt(2));
	m_deferred_lighting_mat->setVariable("gSpecular", &VariableInt(3));

	m_deferred_light_quad->scene_object->setMaterial(m_deferred_lighting_mat);
}

void OpenGLRenderer::initialize_skybox()
{
	std::vector<float> quadVertices = {
		-1.0f,  1.0f,  0.0f,
		-1.0f, -1.0f,  0.0f,
		 1.0f, -1.0f,  0.0f,

		-1.0f,  1.0f,  0.0f,
		 1.0f, -1.0f,  0.0f,
		 1.0f,  1.0f,  0.0f
	};
	std::vector<float> quadTexCoords = {
		 0.0f, 1.0f,
		 0.0f, 0.0f,
		 1.0f, 0.0f,

		 0.0f, 1.0f,
		 1.0f, 0.0f,
		 1.0f, 1.0f
	};
	Mesh* mesh_skybox = new Mesh(quadVertices);
	mesh_skybox->setTextureCoordinates(quadTexCoords);
	Object* obj_skybox = new Object(mesh_skybox);
	create_gl_object(&m_skybox_quad, obj_skybox);

	std::vector<std::string> skybox_faces
	{
		Utils::getAbsolutePathFromXML("core:materials/textures/skybox/px.png"),
		Utils::getAbsolutePathFromXML("core:materials/textures/skybox/nx.png"),
		Utils::getAbsolutePathFromXML("core:materials/textures/skybox/py.png"),
		Utils::getAbsolutePathFromXML("core:materials/textures/skybox/ny.png"),
		Utils::getAbsolutePathFromXML("core:materials/textures/skybox/pz.png"),
		Utils::getAbsolutePathFromXML("core:materials/textures/skybox/nz.png")
	};

	glGenTextures(1, &m_skybox_cubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_skybox_cubemap);

	int width, height, nrComponents;
	for (unsigned int i = 0; i < skybox_faces.size(); i++) {
		unsigned char* data = stbi_load(skybox_faces[i].c_str(), &width, &height, &nrComponents, 4);
		if (data) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else {
			std::cout << "Cubemap texture failed to load at path: " << skybox_faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	m_skybox_mat = new Material();
	m_skybox_mat->addVertexShaderSource(Utils::getAbsolutePathFromXML("core:materials/shaders/skybox_v.glsl").c_str());
	m_skybox_mat->addFragmentShaderSource(Utils::getAbsolutePathFromXML("core:materials/shaders/skybox_f.glsl").c_str());
	m_skybox_mat->apply();
	m_skybox_mat->setVariable("u_texture", &VariableInt(0));
	m_skybox_quad->scene_object->setMaterial(m_skybox_mat);
}

void OpenGLRenderer::initialize_composition_buffer()
{
	std::vector<float> quadVertices = {
		-1.0f,  1.0f,  0.0f,
		-1.0f, -1.0f,  0.0f,
		 1.0f, -1.0f,  0.0f,

		-1.0f,  1.0f,  0.0f,
		 1.0f, -1.0f,  0.0f,
		 1.0f,  1.0f,  0.0f
	};
	std::vector<float> quadTexCoords = {
		 0.0f, 1.0f,
		 0.0f, 0.0f,
		 1.0f, 0.0f,

		 0.0f, 1.0f,
		 1.0f, 0.0f,
		 1.0f, 1.0f
	};

	Mesh* mesh_composition_quad = new Mesh(quadVertices);
	mesh_composition_quad->setTextureCoordinates(quadTexCoords);
	Object* obj_composition_quad = new Object(mesh_composition_quad);
	create_gl_object(&m_composition_quad, obj_composition_quad);

	glGenFramebuffers(1, &m_mainBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_mainBuffer);
	unsigned int attachments[] = { GL_COLOR_ATTACHMENT0 };

	glGenTextures(1, &m_mainBufferOutputColor);
	glBindTexture(GL_TEXTURE_2D, m_mainBufferOutputColor);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Application::get()->getWindowWidth(), Application::get()->getWindowHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachments[0], GL_TEXTURE_2D, m_mainBufferOutputColor, 0);
	glDrawBuffers(sizeof(attachments) / sizeof(unsigned int), attachments);

	glGenRenderbuffers(1, &m_composition_rbo_depth);
	glBindRenderbuffer(GL_RENDERBUFFER, m_composition_rbo_depth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, Application::get()->getWindowWidth(), Application::get()->getWindowHeight());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_composition_rbo_depth);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "mainBuffer Framebuffer not complete!" << std::endl;
	}
	
	m_composition_mat = new Material();
	m_composition_mat->addVertexShaderSource(Utils::getAbsolutePathFromXML("core:materials/shaders/composition_v.glsl").c_str());
	m_composition_mat->addFragmentShaderSource(Utils::getAbsolutePathFromXML("core:materials/shaders/composition_f.glsl").c_str());
	m_composition_mat->apply();
	m_composition_mat->setVariable("u_texture", &VariableInt(0));
	m_composition_quad->scene_object->setMaterial(m_composition_mat);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLRenderer::calculate_geometry()
{
	m_vec_light_dir_indices.clear();
	m_vec_light_point_indices.clear();
	m_vec_light_spot_indices.clear();

	for (int i = 0; i < Game::get()->getNumEntitiesInScene(); ++i) {
		if (dynamic_cast<LightDirectional*>(Game::get()->getEntityFromScene(i))) {
			m_vec_light_dir_indices.push_back(i);
		}
		else if (dynamic_cast<LightPoint*>(Game::get()->getEntityFromScene(i))) {
			m_vec_light_point_indices.push_back(i);
		}
		else if (dynamic_cast<LightSpot*>(Game::get()->getEntityFromScene(i))) {
			m_vec_light_spot_indices.push_back(i);
		}
	}
}

void OpenGLRenderer::geometry_pass()
{
	glEnable(GL_DEPTH_TEST);
	renderObjects();
}

void OpenGLRenderer::lighting_pass()
{
	m_deferred_light_quad->scene_object->getMaterial()->use();

	m_deferred_light_quad->scene_object->getMaterial()->setVariable("viewPos", &VariableVec3(Game::get()->getCamera()->getPosition()));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_gBufferPosition);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_gBufferNormal);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_gBufferAlbedo);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m_gBufferSpecular);

	m_deferred_light_quad->scene_object->getMaterial()->setVariable(
		"n_lights_directional",
		&VariableInt(m_vec_light_dir_indices.size()));
	m_deferred_light_quad->scene_object->getMaterial()->setVariable(
		"n_lights_point",
		&VariableInt(m_vec_light_point_indices.size()));
	m_deferred_light_quad->scene_object->getMaterial()->setVariable(
		"n_lights_spot",
		&VariableInt(m_vec_light_spot_indices.size()));

	for (unsigned int i = 0; i < m_vec_light_dir_indices.size(); i++) {
		LightDirectional* light_dir = dynamic_cast<LightDirectional*>(Game::get()->getEntityFromScene(m_vec_light_dir_indices[i]));
		m_deferred_light_quad->scene_object->getMaterial()->setVariable(
			("lights_directional[" + Utils::intToString(i) + "].direction").c_str(),
			&VariableVec3(light_dir->getDirection()));
		m_deferred_light_quad->scene_object->getMaterial()->setVariable(
			("lights_directional[" + Utils::intToString(i) + "].ambient").c_str(),
			&VariableVec3(light_dir->getAmbient()));
		m_deferred_light_quad->scene_object->getMaterial()->setVariable(
			("lights_directional[" + Utils::intToString(i) + "].diffuse").c_str(),
			&VariableVec3(light_dir->getDiffuse()));
		m_deferred_light_quad->scene_object->getMaterial()->setVariable(
			("lights_directional[" + Utils::intToString(i) + "].specular").c_str(),
			&VariableVec3(light_dir->getSpecular()));
	}

	for (unsigned int i = 0; i < m_vec_light_point_indices.size(); i++) {
		LightPoint* light_point = dynamic_cast<LightPoint*>(Game::get()->getEntityFromScene(m_vec_light_point_indices[i]));
		m_deferred_light_quad->scene_object->getMaterial()->setVariable(
			("lights_point[" + Utils::intToString(i) + "].position").c_str(),
			&VariableVec3(light_point->getPosition()));
		m_deferred_light_quad->scene_object->getMaterial()->setVariable(
			("lights_point[" + Utils::intToString(i) + "].ambient").c_str(),
			&VariableVec3(light_point->getAmbient()));
		m_deferred_light_quad->scene_object->getMaterial()->setVariable(
			("lights_point[" + Utils::intToString(i) + "].diffuse").c_str(),
			&VariableVec3(light_point->getDiffuse()));
		m_deferred_light_quad->scene_object->getMaterial()->setVariable(
			("lights_point[" + Utils::intToString(i) + "].specular").c_str(),
			&VariableVec3(light_point->getSpecular()));
		m_deferred_light_quad->scene_object->getMaterial()->setVariable(
			("lights_point[" + Utils::intToString(i) + "].constant").c_str(),
			&VariableFloat(light_point->getConstant()));
		m_deferred_light_quad->scene_object->getMaterial()->setVariable(
			("lights_point[" + Utils::intToString(i) + "].linear").c_str(),
			&VariableFloat(light_point->getLinear()));
		m_deferred_light_quad->scene_object->getMaterial()->setVariable(
			("lights_point[" + Utils::intToString(i) + "].quadratic").c_str(),
			&VariableFloat(light_point->getQuadratic()));
	}

	for (unsigned int i = 0; i < m_vec_light_spot_indices.size(); i++) {
		//LightSpot* light_spot = dynamic_cast<LightSpot*>(Game::get()->getEntityFromScene(m_vec_light_spot_indices[i]));
	}

	glBindVertexArray(m_deferred_light_quad->VAO);
	glDrawArrays(GL_TRIANGLES, 0, m_deferred_light_quad->scene_object->getMesh()->getVertices().size() / 3);
	glBindVertexArray(0);
}

void OpenGLRenderer::depth_pass(unsigned int src_buf, unsigned int dest_buf)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, src_buf);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dest_buf);
	glBlitFramebuffer(0, 0, Application::get()->getWindowWidth(), Application::get()->getWindowHeight(),
		0, 0, Application::get()->getWindowWidth(), Application::get()->getWindowHeight(), GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, dest_buf);
}

void OpenGLRenderer::render_lights()
{
	// todo render light objects
}

void OpenGLRenderer::render_skybox()
{
	glDepthFunc(GL_LEQUAL);
	m_skybox_mat->setVariable("u_camera_view", &VariableMat4(Game::get()->getCamera()->getView()));
	m_skybox_mat->setVariable("u_camera_projection", &VariableMat4(Game::get()->getCamera()->getProjection()));
	m_skybox_quad->scene_object->getMaterial()->use();
	// skybox cube
	glBindVertexArray(m_skybox_quad->VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_skybox_cubemap);
	glDrawArrays(GL_TRIANGLES, 0, m_skybox_quad->scene_object->getMesh()->getVertices().size() / 3);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); // set depth function back to default
}

void OpenGLRenderer::compose_passes()
{
	m_composition_quad->scene_object->getMaterial()->use();
	glBindVertexArray(m_composition_quad->VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_mainBufferOutputColor);
	glDrawArrays(GL_TRIANGLES, 0, m_composition_quad->scene_object->getMesh()->getVertices().size() / 3);
	glBindVertexArray(0);
}

void OpenGLRenderer::postprocess()
{
}

void OpenGLRenderer::create_gl_object(OpenGLObject** gl_object, Object* obj) {
    if (obj->getMesh() && !obj->getMesh()->getVertices().empty()) {
        int num_attrib_pointer = 0;

        *gl_object = new OpenGLObject();
        OpenGLObject* gl_obj = *gl_object;
        gl_obj->scene_object = obj;

        glGenVertexArrays(1, &gl_obj->VAO);
        gl_obj->vec_VBOs.push_back(0);
        glGenBuffers(1, &gl_obj->vec_VBOs.back()); // vertices

        glBindVertexArray(gl_obj->VAO);

        glBindBuffer(GL_ARRAY_BUFFER, gl_obj->vec_VBOs.back());
        glBufferData(GL_ARRAY_BUFFER,
            gl_obj->scene_object->getMesh()->getVertices().size() * sizeof(float),
            gl_obj->scene_object->getMesh()->getVertices().data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(num_attrib_pointer);
        glVertexAttribPointer(num_attrib_pointer++, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
        glDisable(GL_ARRAY_BUFFER);

        if (!gl_obj->scene_object->getMesh()->getNormals().empty()) {
            gl_obj->vec_VBOs.push_back(0);
            glGenBuffers(1, &gl_obj->vec_VBOs.back()); // normals

            glBindBuffer(GL_ARRAY_BUFFER, gl_obj->vec_VBOs.back());
            glBufferData(GL_ARRAY_BUFFER,
                gl_obj->scene_object->getMesh()->getNormals().size() * sizeof(float),
                gl_obj->scene_object->getMesh()->getNormals().data(), GL_STATIC_DRAW);
            glEnableVertexAttribArray(num_attrib_pointer);
            glVertexAttribPointer(num_attrib_pointer++, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
            glDisable(GL_ARRAY_BUFFER);
        }

        if (!gl_obj->scene_object->getMesh()->getTextureCoordinates().empty()) {
            gl_obj->vec_VBOs.push_back(0);
            glGenBuffers(1, &gl_obj->vec_VBOs.back()); // texture coordinates

            glBindBuffer(GL_ARRAY_BUFFER, gl_obj->vec_VBOs.back());
            glBufferData(GL_ARRAY_BUFFER,
                gl_obj->scene_object->getMesh()->getTextureCoordinates().size() * sizeof(float),
                gl_obj->scene_object->getMesh()->getTextureCoordinates().data(), GL_STATIC_DRAW);
            glEnableVertexAttribArray(num_attrib_pointer);
            glVertexAttribPointer(num_attrib_pointer++, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
            glDisable(GL_ARRAY_BUFFER);
        }
        glBindVertexArray(0);
    }
}
