// applicaiton.cpp

#include "spinach.hpp"

#include <GLFW/glfw3.h> // keycodes, ...
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

application::application(const char *title, int width, int height)
   : m_context(title, width, height, this)
{
}

application::~application()
{
}

void application::run()
{
   m_running = m_context.valid();
   while (m_running && m_context.poll_events()) {
      m_backend.clear(0.1f, 0.2f, 0.3f, 1.0f);
      // note: rendering goes here!
      m_context.swap_buffers();
   }
}

void application::on_key(int key, bool state)
{
   if (key == GLFW_KEY_ESCAPE && !state) {
      m_running = false;
   }
}

void application::on_mouse(int x, int y)
{
}

void application::on_button(int button, bool state) 
{

}
