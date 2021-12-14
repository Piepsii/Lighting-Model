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

	void CalculateTangentsAndBitangents(glm::vec3& pos2, glm::vec3& pos1, glm::vec3& pos3, glm::vec2& uv2, glm::vec2& uv1, glm::vec2& uv3);

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



