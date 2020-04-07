#ifndef AETNA_APPLICATION_GL_H
#define AETNA_APPLICATION_GL_H

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <framework/Application.h>

#include <chrono>

class ApplicationGL : public Application {
public:
	ApplicationGL();
	virtual ~ApplicationGL();

	void initialize(int init_width, int init_height, const char* init_title) override;
	void preUpdate() override;
	void update() override;
	void postUpdate() override;
	void swap() override;
	void terminate() override;

	bool windowIsOpen() override;
	int getWindowWidth() override;
	int getWindowHeight() override;
	void resizeWindow(int w, int h) override;
	const char* getWindowTitle() override;
	void setWindowTitle(const char* title) override;

	void setVerticalSync(int value) override;

	float getTime() override;
	float getDeltaTime() override;
	int getFPS() override;

	GLFWwindow* getWindow();

protected:
	GLFWwindow* m_main_window = nullptr;

	std::chrono::time_point<std::chrono::system_clock> start_time;
	std::chrono::time_point<std::chrono::system_clock> frame_time;
	float last_time;
	float delta_time;
	int t_fps_count;
	int fps_count;

	void initialize_input();
	void initialize_renderer();

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};

#endif //AETNA_APPLICATION_GL_H