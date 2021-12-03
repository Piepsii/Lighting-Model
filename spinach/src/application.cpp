// Application.cpp

#include "Spinach.h"

#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct vertex {
	float x, y, z;
	float u, v;
};

const vertex cube_data[360] = {
	// front
   { -1.0f,  1.0f,  1.0f,   0.0f, 1.0f, },
   {  1.0f,  1.0f,  1.0f,   1.0f, 1.0f, },
   {  1.0f, -1.0f,  1.0f,   1.0f, 0.0f, },
   {  1.0f, -1.0f,  1.0f,   1.0f, 0.0f, },
   { -1.0f, -1.0f,  1.0f,   0.0f, 0.0f, },
   { -1.0f,  1.0f,  1.0f,   0.0f, 1.0f, },

   // right                             
   {  1.0f,  1.0f,  1.0f,   0.0f, 1.0f, },
   {  1.0f,  1.0f, -1.0f,   1.0f, 1.0f, },
   {  1.0f, -1.0f, -1.0f,   1.0f, 0.0f, },
   {  1.0f, -1.0f, -1.0f,   1.0f, 0.0f, },
   {  1.0f, -1.0f,  1.0f,   0.0f, 0.0f, },
   {  1.0f,  1.0f,  1.0f,   0.0f, 1.0f, },

   // back                              
   {  1.0f,  1.0f, -1.0f,   0.0f, 1.0f, },
   { -1.0f,  1.0f, -1.0f,   1.0f, 1.0f, },
   { -1.0f, -1.0f, -1.0f,   1.0f, 0.0f, },
   { -1.0f, -1.0f, -1.0f,   1.0f, 0.0f, },
   {  1.0f, -1.0f, -1.0f,   0.0f, 0.0f, },
   {  1.0f,  1.0f, -1.0f,   0.0f, 1.0f, },

   // left                              
   { -1.0f,  1.0f, -1.0f,   0.0f, 1.0f, },
   { -1.0f,  1.0f,  1.0f,   1.0f, 1.0f, },
   { -1.0f, -1.0f,  1.0f,   1.0f, 0.0f, },
   { -1.0f, -1.0f,  1.0f,   1.0f, 0.0f, },
   { -1.0f, -1.0f, -1.0f,   0.0f, 0.0f, },
   { -1.0f,  1.0f, -1.0f,   0.0f, 1.0f, },

   // top                               
   {  1.0f,  1.0f,  1.0f,   0.0f, 1.0f, },
   { -1.0f,  1.0f,  1.0f,   1.0f, 1.0f, },
   { -1.0f,  1.0f, -1.0f,   1.0f, 0.0f, },
   { -1.0f,  1.0f, -1.0f,   1.0f, 0.0f, },
   {  1.0f,  1.0f, -1.0f,   0.0f, 0.0f, },
   {  1.0f,  1.0f,  1.0f,   0.0f, 1.0f, },

   // bottom                               
   { -1.0f, -1.0f,  1.0f,   0.0f, 1.0f, },
   {  1.0f, -1.0f,  1.0f,   1.0f, 1.0f, },
   {  1.0f, -1.0f, -1.0f,   1.0f, 0.0f, },
   {  1.0f, -1.0f, -1.0f,   1.0f, 0.0f, },
   { -1.0f, -1.0f, -1.0f,   0.0f, 0.0f, },
   { -1.0f, -1.0f,  1.0f,   0.0f, 1.0f, },
};

static const float final_data[]{
   -1.0f,  1.0f,  0.0f, 1.0f,
	1.0f,  1.0f,  1.0f, 1.0f,
	1.0f, -1.0f,  1.0f, 0.0f,
	1.0f, -1.0f,  1.0f, 0.0f,
   -1.0f, -1.0f,  0.0f, 0.0f,
   -1.0f,  1.0f,  0.0f, 1.0f,
};


Application::Application(const char* title, int width, int height)
	: context(title, width, height, this),
	windowWidth(width),
	windowHeight(height) {
}

Application::~Application() {

}

void Application::Run() {
	running = context.IsValid();

	while (running && context.PollEvents()) {
		// rendering goes here
		context.SwapBuffers();
	}
}

void Application::OnKey(int key, bool state) {
	if (key == GLFW_KEY_ESCAPE && !state) {
		running = false;
	}
}

void Application::OnMouse(int x, int y) {
}

void Application::OnScroll(int xoffset, int yoffset) {
}

void Application::OnButton(int button, bool state) {
}