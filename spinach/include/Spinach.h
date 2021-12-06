// Spinach.h

#pragma once

#include "Utility.h"

#include <vector>


struct GLFWwindow;
struct RenderContext {
	RenderContext(const char* title, int width, int height, void* user_data);
	~RenderContext();
	
	bool IsValid() const;
	bool PollEvents();
	void SwapBuffers();

	GLFWwindow* window{};
};

struct Application {
	Application(const char* title, int width, int height);
	~Application();

	void Run();

	void OnKey(int key, bool state);
	void OnMouse(int x, int y);
	void OnScroll(int xoffset, int yoffset);
	void OnButton(int button, bool state);

	bool running{};
	RenderContext context;
	RenderBackend backend;
	Utility::Time time;
	Utility::Mouse mouse;
	Utility::Keyboard keyboard;

	int windowWidth, windowHeight;
};



