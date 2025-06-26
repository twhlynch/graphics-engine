#include "Window.hpp"

#include "../Logging.hpp"

#include <GLFW/glfw3.h>

std::map<int, Window::KeyState> Window::_keys;

Window::Window(int width, int height, const std::string &title) :
	_window(nullptr), _windowActive(true)
{
	if (!glfwInit())
	{
		ERROR("Failed to initialize GLFW");
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	if (!_window)
	{
		ERROR("Failed to create GLFW window");
		glfwTerminate();
	}

	glfwMakeContextCurrent(_window);
	glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);
	glfwSetKeyCallback(_window, key_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		ERROR("Failed to initialize GLAD");
	}

	glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

Window::~Window()
{
	glfwDestroyWindow(_window);
	glfwTerminate();
}

void Window::pollEvents()
{
	glfwPollEvents();
}

void Window::swapBuffers() const
{
	glfwSwapBuffers(_window);
}

bool Window::shouldClose() const
{
	return glfwWindowShouldClose(_window);
}

void Window::framebuffer_size_callback(GLFWwindow * /*window*/, int width, int height)
{
	glViewport(0, 0, width, height);
}


void Window::key_callback(GLFWwindow * /*window*/, int key, int /*scancode*/, int action, int /*mods*/)
{
	if (action == GLFW_PRESS)
	{
		KeyState &state = _keys[key];
		if (state == RELEASED)
		{
			state = PRESSED;
		}
	}
	else if (action == GLFW_RELEASE)
	{
		_keys[key] = RELEASED;
	}
}

bool Window::isPressing(int key)
{
	return Window::_keys[key] == PRESSED;
}
bool Window::isHolding(int key)
{
	return Window::_keys[key] == HELD;
}

void Window::update()
{
	glfwSwapBuffers(_window);

	for (auto pair : Window::_keys)
	{
		if (pair.second == PRESSED && glfwGetKey(_window, pair.first) == GLFW_PRESS)
		{
			Window::_keys[pair.first] = HELD;
		}
	}
	glfwPollEvents();

	if (isPressing(GLFW_KEY_ESCAPE))
	{
		_windowActive = !_windowActive;
		if (_windowActive)
		{
			glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		else
		{
			glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}
}
