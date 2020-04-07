#include "ApplicationGL.h"

#include <framework/input/raw/InputGLFW.h>
#include <framework/render/OpenGLRenderer.h>

#include <iostream>

ApplicationGL::ApplicationGL() : Application(this) {
	start_time = std::chrono::system_clock::now();
	frame_time = std::chrono::system_clock::now();
	float last_time = getTime();
	float delta_time = 0.0f;
	int t_fps_count = 0;
	int fps_count = 0;
}

ApplicationGL::~ApplicationGL() {

}

void ApplicationGL::initialize(int init_width, int init_height, const char* init_title) {
	if (!glfwInit()) {
		std::cout << "Failed to initialize GLFW" << std::endl;
		exit(EXIT_FAILURE);
	}

	m_main_window = glfwCreateWindow(init_width, init_height,
		init_title, nullptr, nullptr);

	if (m_main_window == nullptr) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(m_main_window);

	initialize_input();
	initialize_renderer();
	if (!m_renderer->isInitialized()) {
		std::cerr << "Failed to initialize Renderer" << std::endl;
		terminate();
		return;
	}
}

void ApplicationGL::preUpdate() {
	++t_fps_count;
	delta_time = std::chrono::duration<float>(std::chrono::system_clock::now() - frame_time).count();
	frame_time = std::chrono::system_clock::now();

	m_input_proxy->updateInput();
}

void ApplicationGL::update() {
	m_renderer->update();
}

void ApplicationGL::postUpdate() {
	m_renderer->render();
	swap();

	if (getTime() - last_time >= 1.0f) {
		last_time = getTime();
		fps_count = t_fps_count;
		t_fps_count = 0;
	}
}

void ApplicationGL::swap() {
	glfwPollEvents();
	glfwSwapBuffers(m_main_window);
}

void ApplicationGL::terminate() {
	if (m_renderer) {
		delete m_renderer;
		m_renderer = nullptr;
	}
	glfwTerminate();
}

bool ApplicationGL::windowIsOpen() {
	if (!m_main_window) {
		return false;
	}
	return !glfwWindowShouldClose(m_main_window);
}

int ApplicationGL::getWindowWidth() {
	int width = 0, height = 0;
	glfwGetWindowSize(m_main_window, &width, &height);
	return width;
}

int ApplicationGL::getWindowHeight() {
	int width = 0, height = 0;
	glfwGetWindowSize(m_main_window, &width, &height);
	return height;
}

const char* ApplicationGL::getWindowTitle() {
	return m_window_title.c_str();
}

void ApplicationGL::setWindowTitle(const char* title) {
	m_window_title = title;
	glfwSetWindowTitle(m_main_window, m_window_title.c_str());
}

void ApplicationGL::setVerticalSync(int value) {
	glfwSwapInterval(value);
}

float ApplicationGL::getTime() {
	return std::chrono::duration<float>(std::chrono::system_clock::now() - start_time).count();
}

float ApplicationGL::getDeltaTime() {
	return delta_time;
}

int ApplicationGL::getFPS() {
	return fps_count;
}

GLFWwindow* ApplicationGL::getWindow() {
	return m_main_window;
}

void ApplicationGL::resizeWindow(int w, int h) {
	glfwSetWindowSize(m_main_window, w, h);
}

void ApplicationGL::initialize_input() {
	m_input_proxy = new InputGLFW(getWindow());
}

void ApplicationGL::initialize_renderer() {
	m_renderer = new OpenGLRenderer();
	m_renderer->initialize(glfwGetProcAddress);

	glfwSetFramebufferSizeCallback(m_main_window, framebuffer_size_callback);
}

void ApplicationGL::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	OpenGLRenderer* m_renderer_gl = dynamic_cast<OpenGLRenderer*>(Application::get()->getRenderer());
	m_renderer_gl->resizeViewport(width, height);
}
