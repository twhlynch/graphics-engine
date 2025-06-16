#pragma once

#include <glad/glad.h>

#include <map>
#include <string>

struct GLFWwindow;

class Window
{
	enum KeyState
	{
		RELEASED,
		HELD,
		PRESSED,
	};

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

	bool isPressing(int key);
	bool isHolding(int key);

	static std::map<int, KeyState> _keys;

private:
	static void framebuffer_size_callback(GLFWwindow *window, int width, int height);
	static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

	GLFWwindow *_window;
	bool _windowActive;
};
