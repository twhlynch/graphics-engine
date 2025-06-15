#pragma once

#include <glad/glad.h>

#include <string>

struct GLFWwindow;

class Window
{
public:
	Window(int width, int height, const std::string &title);
	~Window();

	void pollEvents() const;
	void swapBuffers() const;
	bool shouldClose() const;
	void update();

	GLFWwindow *getGLFWwindow() const
	{
		return _window;
	}
	bool getWindowActive() const
	{
		return _windowActive;
	}

private:
	static void framebuffer_size_callback(GLFWwindow *window, int width, int height);

	GLFWwindow *_window;
	bool _windowActive;
	bool _pressingEscape;
};
