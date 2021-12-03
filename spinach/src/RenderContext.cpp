// RenderContext.cpp

#include "Spinach.h"
#include <cstdio>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	Application* app = (Application*)glfwGetWindowUserPointer(window);
	if (app == nullptr) {
		return;
	}


	app->OnKey(key, action != GLFW_RELEASE);
}

static void MouseCallback(GLFWwindow* window, double xpos, double ypos) {
	Application* app = (Application*)glfwGetWindowUserPointer(window);
	if (app == nullptr) {
		return;
	}
	app->OnMouse(int(xpos), int(ypos));
}

static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	Application* app = (Application*)glfwGetWindowUserPointer(window);
	if (app == nullptr) {
		return;
	}
	app->OnScroll(int(xoffset), int(yoffset));
}

static void ButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	Application* app = (Application*)glfwGetWindowUserPointer(window);
	if (app == nullptr) {
		return;
	}
	app->OnButton(button, action == GLFW_PRESS);
}

RenderContext::RenderContext(const char* title, int width, int height, void* user_data)
: window(nullptr) {
	glfwSetErrorCallback([](int code, const char* message) {
		printf("!!! %s (%d)\n", message, code);
	});

	if (!glfwInit()) {
		return;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLFWwindow* _window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (_window == nullptr) {
		return;
	}

	glfwMakeContextCurrent(_window);
	glfwSwapInterval(1);
	if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0) {
		glfwDestroyWindow(_window);
		return;
	}

	glfwSetKeyCallback(_window, KeyCallback);
	glfwSetCursorPosCallback(_window, MouseCallback);
	glfwSetScrollCallback(_window, ScrollCallback);
	glfwSetMouseButtonCallback(_window, ButtonCallback);
	glfwSetWindowUserPointer(_window, user_data);

	window = _window;
}

RenderContext::~RenderContext() {
	glfwDestroyWindow(window);
	glfwTerminate();
}

bool RenderContext::IsValid() const {
	return window != nullptr;
}

bool RenderContext::PollEvents() {
	if (!IsValid()) {
		return false;
	}
	glfwPollEvents();
	return !glfwWindowShouldClose(window);
}

void RenderContext::SwapBuffers() {
	if (!IsValid()) {
		return;
	}
	glfwSwapBuffers(window);
}
