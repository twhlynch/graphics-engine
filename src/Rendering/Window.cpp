#include "Window.hpp"

#include "../Logging.hpp"

#include <GLFW/glfw3.h>

#include <iostream>

Window::Window(int width, int height, const std::string &title)
{
	if (!glfwInit())
	{
		ERROR("Failed to initialize GLFW");
		std::exit(EXIT_FAILURE);
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
		std::exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(_window);
	glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		ERROR("Failed to initialize GLAD");
		std::exit(EXIT_FAILURE);
	}

	glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

Window::~Window()
{
	glfwDestroyWindow(_window);
	glfwTerminate();
}

void Window::pollEvents() const
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

void Window::update() const
{
	glfwSwapBuffers(_window);
	glfwPollEvents();

	if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(_window, true);
}

GLFWwindow *Window::getGLFWwindow() const
{
	return _window;
}