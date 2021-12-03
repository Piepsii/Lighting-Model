// spinach.hpp

#pragma once

#include <render.hpp>
#include <glm/glm.hpp> // vecN,matN,quat

struct GLFWwindow;
struct render_context {
   render_context(const char *title, int width, int height, void *userdata);
   ~render_context();

   bool valid() const;
   bool poll_events();
   void swap_buffers();

   GLFWwindow *m_window{};
};

struct application {
   application(const char *title, int width, int height);
   ~application();

   void run();

   void on_key(int key, bool state);
   void on_mouse(int x, int y);
   void on_button(int button, bool state);

   bool m_running{};
   render_context m_context;
   render_backend m_backend;
};

namespace utility
{
   // note: milliseconds since process start
   int64 get_current_tick();

   bool create_shader_program_from_files(shader_program &program,
                                         const char *vertex_filename,
                                         const char *fragment_filename);

   bool create_texture_from_file(texture &texture,
                                 const char *filename);
} // !utility
