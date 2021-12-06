// Application.cpp

#include "Spinach.h"

#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct vertex {
	float x, y, z;
	float r, g, b, a;
	float u, v;
	float nx, ny, nz;
};

const vertex cube_data[36] = {
	// front
   { -1.0f,  1.0f,  1.0f,   1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 1.0f,		 0.0f, 0.0f, 1.0f, },
   {  1.0f,  1.0f,  1.0f,   1.0f, 1.0f, 1.0f, 1.0f,		1.0f, 1.0f,		 0.0f, 0.0f, 1.0f, },
   {  1.0f, -1.0f,  1.0f,   1.0f, 1.0f, 1.0f, 1.0f,		1.0f, 0.0f,		 0.0f, 0.0f, 1.0f, },
   {  1.0f, -1.0f,  1.0f,   1.0f, 1.0f, 1.0f, 1.0f,		1.0f, 0.0f,		 0.0f, 0.0f, 1.0f, },
   { -1.0f, -1.0f,  1.0f,   1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 0.0f,		 0.0f, 0.0f, 1.0f, },
   { -1.0f,  1.0f,  1.0f,   1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 1.0f,		 0.0f, 0.0f, 1.0f, },

   // right                             
   {  1.0f,  1.0f,  1.0f,   1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 1.0f,		 1.0f, 0.0f, 0.0f, },
   {  1.0f,  1.0f, -1.0f,   1.0f, 1.0f, 1.0f, 1.0f,		1.0f, 1.0f,		 1.0f, 0.0f, 0.0f, },
   {  1.0f, -1.0f, -1.0f,   1.0f, 1.0f, 1.0f, 1.0f,		1.0f, 0.0f,		 1.0f, 0.0f, 0.0f, },
   {  1.0f, -1.0f, -1.0f,   1.0f, 1.0f, 1.0f, 1.0f,		1.0f, 0.0f,		 1.0f, 0.0f, 0.0f, },
   {  1.0f, -1.0f,  1.0f,   1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 0.0f,		 1.0f, 0.0f, 0.0f, },
   {  1.0f,  1.0f,  1.0f,   1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 1.0f,		 1.0f, 0.0f, 0.0f, },

   // back                              
   {  1.0f,  1.0f, -1.0f,   1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 1.0f,		 0.0f, 0.0f,-1.0f,  },
   { -1.0f,  1.0f, -1.0f,   1.0f, 1.0f, 1.0f, 1.0f,		1.0f, 1.0f,		 0.0f, 0.0f,-1.0f,  },
   { -1.0f, -1.0f, -1.0f,   1.0f, 1.0f, 1.0f, 1.0f,		1.0f, 0.0f,		 0.0f, 0.0f,-1.0f,  },
   { -1.0f, -1.0f, -1.0f,   1.0f, 1.0f, 1.0f, 1.0f,		1.0f, 0.0f,		 0.0f, 0.0f,-1.0f,  },
   {  1.0f, -1.0f, -1.0f,   1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 0.0f,		 0.0f, 0.0f,-1.0f,  },
   {  1.0f,  1.0f, -1.0f,   1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 1.0f,		 0.0f, 0.0f,-1.0f,  },

   // left                              
   { -1.0f,  1.0f, -1.0f,   1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 1.0f,		-1.0f, 0.0f, 0.0f,  },
   { -1.0f,  1.0f,  1.0f,   1.0f, 1.0f, 1.0f, 1.0f,		1.0f, 1.0f,		-1.0f, 0.0f, 0.0f,  },
   { -1.0f, -1.0f,  1.0f,   1.0f, 1.0f, 1.0f, 1.0f,		1.0f, 0.0f,		-1.0f, 0.0f, 0.0f,  },
   { -1.0f, -1.0f,  1.0f,   1.0f, 1.0f, 1.0f, 1.0f,		1.0f, 0.0f,		-1.0f, 0.0f, 0.0f,  },
   { -1.0f, -1.0f, -1.0f,   1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 0.0f,		-1.0f, 0.0f, 0.0f,  },
   { -1.0f,  1.0f, -1.0f,   1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 1.0f,		-1.0f, 0.0f, 0.0f,  },

   // top                               
   {  1.0f,  1.0f,  1.0f,   1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 1.0f,		 0.0f, 1.0f, 0.0f,  },
   { -1.0f,  1.0f,  1.0f,   1.0f, 1.0f, 1.0f, 1.0f,		1.0f, 1.0f,		 0.0f, 1.0f, 0.0f,  },
   { -1.0f,  1.0f, -1.0f,   1.0f, 1.0f, 1.0f, 1.0f,		1.0f, 0.0f,		 0.0f, 1.0f, 0.0f,  },
   { -1.0f,  1.0f, -1.0f,   1.0f, 1.0f, 1.0f, 1.0f,		1.0f, 0.0f,		 0.0f, 1.0f, 0.0f,  },
   {  1.0f,  1.0f, -1.0f,   1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 0.0f,		 0.0f, 1.0f, 0.0f,  },
   {  1.0f,  1.0f,  1.0f,   1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 1.0f,		 0.0f, 1.0f, 0.0f,  },

   // bottom                               
   { -1.0f, -1.0f,  1.0f,   1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 1.0f,		 0.0f,-1.0f, 0.0f,  },
   {  1.0f, -1.0f,  1.0f,   1.0f, 1.0f, 1.0f, 1.0f,		1.0f, 1.0f,		 0.0f,-1.0f, 0.0f,  },
   {  1.0f, -1.0f, -1.0f,   1.0f, 1.0f, 1.0f, 1.0f,		1.0f, 0.0f,		 0.0f,-1.0f, 0.0f,  },
   {  1.0f, -1.0f, -1.0f,   1.0f, 1.0f, 1.0f, 1.0f,		1.0f, 0.0f,		 0.0f,-1.0f, 0.0f,  },
   { -1.0f, -1.0f, -1.0f,   1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 0.0f,		 0.0f,-1.0f, 0.0f,  },
   { -1.0f, -1.0f,  1.0f,   1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 1.0f,		 0.0f,-1.0f, 0.0f,  },
};

const vertex floor_data[36] = {
	// front
   { -5.0f,  1.0f,  5.0f,   1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 0.4f,		 0.0f, 0.0f, 1.0f, },
   {  5.0f,  1.0f,  5.0f,   1.0f, 1.0f, 1.0f, 1.0f,		2.0f, 0.4f,		 0.0f, 0.0f, 1.0f, },
   {  5.0f, -1.0f,  5.0f,   1.0f, 1.0f, 1.0f, 1.0f,		2.0f, 0.0f,		 0.0f, 0.0f, 1.0f, },
   {  5.0f, -1.0f,  5.0f,   1.0f, 1.0f, 1.0f, 1.0f,		2.0f, 0.0f,		 0.0f, 0.0f, 1.0f, },
   { -5.0f, -1.0f,  5.0f,   1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 0.0f,		 0.0f, 0.0f, 1.0f, },
   { -5.0f,  1.0f,  5.0f,   1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 0.4f,		 0.0f, 0.0f, 1.0f, },

   // right                             
   {  5.0f,  1.0f,  5.0f,   1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 0.4f,		 1.0f, 0.0f, 0.0f, },
   {  5.0f,  1.0f, -5.0f,   1.0f, 1.0f, 1.0f, 1.0f,		2.0f, 0.4f,		 1.0f, 0.0f, 0.0f, },
   {  5.0f, -1.0f, -5.0f,   1.0f, 1.0f, 1.0f, 1.0f,		2.0f, 0.0f,		 1.0f, 0.0f, 0.0f, },
   {  5.0f, -1.0f, -5.0f,   1.0f, 1.0f, 1.0f, 1.0f,		2.0f, 0.0f,		 1.0f, 0.0f, 0.0f, },
   {  5.0f, -1.0f,  5.0f,   1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 0.0f,		 1.0f, 0.0f, 0.0f, },
   {  5.0f,  1.0f,  5.0f,   1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 0.4f,		 1.0f, 0.0f, 0.0f, },

   // back                              
   {  5.0f,  1.0f, -5.0f,   1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 0.4f,		 0.0f, 0.0f,-1.0f,  },
   { -5.0f,  1.0f, -5.0f,   1.0f, 1.0f, 1.0f, 1.0f,		2.0f, 0.4f,		 0.0f, 0.0f,-1.0f,  },
   { -5.0f, -1.0f, -5.0f,   1.0f, 1.0f, 1.0f, 1.0f,		2.0f, 0.0f,		 0.0f, 0.0f,-1.0f,  },
   { -5.0f, -1.0f, -5.0f,   1.0f, 1.0f, 1.0f, 1.0f,		2.0f, 0.0f,		 0.0f, 0.0f,-1.0f,  },
   {  5.0f, -1.0f, -5.0f,   1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 0.0f,		 0.0f, 0.0f,-1.0f,  },
   {  5.0f,  1.0f, -5.0f,   1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 0.4f,		 0.0f, 0.0f,-1.0f,  },

   // left                              
   { -5.0f,  1.0f, -5.0f,   1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 0.4f,		-1.0f, 0.0f, 0.0f,  },
   { -5.0f,  1.0f,  5.0f,   1.0f, 1.0f, 1.0f, 1.0f,		2.0f, 0.4f,		-1.0f, 0.0f, 0.0f,  },
   { -5.0f, -1.0f,  5.0f,   1.0f, 1.0f, 1.0f, 1.0f,		2.0f, 0.0f,		-1.0f, 0.0f, 0.0f,  },
   { -5.0f, -1.0f,  5.0f,   1.0f, 1.0f, 1.0f, 1.0f,		2.0f, 0.0f,		-1.0f, 0.0f, 0.0f,  },
   { -5.0f, -1.0f, -5.0f,   1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 0.0f,		-1.0f, 0.0f, 0.0f,  },
   { -5.0f,  1.0f, -5.0f,   1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 0.4f,		-1.0f, 0.0f, 0.0f,  },

   // top                               
   {  5.0f,  1.0f,  5.0f,   1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 2.0f,		 0.0f, 1.0f, 0.0f,  },
   { -5.0f,  1.0f,  5.0f,   1.0f, 1.0f, 1.0f, 1.0f,		2.0f, 2.0f,		 0.0f, 1.0f, 0.0f,  },
   { -5.0f,  1.0f, -5.0f,   1.0f, 1.0f, 1.0f, 1.0f,		2.0f, 0.0f,		 0.0f, 1.0f, 0.0f,  },
   { -5.0f,  1.0f, -5.0f,   1.0f, 1.0f, 1.0f, 1.0f,		2.0f, 0.0f,		 0.0f, 1.0f, 0.0f,  },
   {  5.0f,  1.0f, -5.0f,   1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 0.0f,		 0.0f, 1.0f, 0.0f,  },
   {  5.0f,  1.0f,  5.0f,   1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 2.0f,		 0.0f, 1.0f, 0.0f,  },

   // bottom                               
   { -5.0f, -1.0f,  5.0f,   1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 2.0f,		 0.0f,-1.0f, 0.0f,  },
   {  5.0f, -1.0f,  5.0f,   1.0f, 1.0f, 1.0f, 1.0f,		2.0f, 2.0f,		 0.0f,-1.0f, 0.0f,  },
   {  5.0f, -1.0f, -5.0f,   1.0f, 1.0f, 1.0f, 1.0f,		2.0f, 0.0f,		 0.0f,-1.0f, 0.0f,  },
   {  5.0f, -1.0f, -5.0f,   1.0f, 1.0f, 1.0f, 1.0f,		2.0f, 0.0f,		 0.0f,-1.0f, 0.0f,  },
   { -5.0f, -1.0f, -5.0f,   1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 0.0f,		 0.0f,-1.0f, 0.0f,  },
   { -5.0f, -1.0f,  5.0f,   1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 2.0f,		 0.0f,-1.0f, 0.0f,  },
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

	const FramebufferFormat formats[] = {
		FRAMEBUFFER_FORMAT_RGBA8,
		FRAMEBUFFER_FORMAT_D32
	};
	Framebuffer rendertarget;
	if (!rendertarget.Create(1920, 1080, 2, formats)) {
		return;
	}
	ShaderProgram world_program;
	if (!Utility::CreateShaderProgramFromFiles(world_program,
											   "data/world.vs.glsl",
											   "data/world.fs.glsl")) {
		return;
	}
	Texture grass_texture;
	if (!Utility::CreateTextureFromFile(grass_texture,
										"data/grass.png")) {
		return;
	}
	Texture crate_texture;
	if (!Utility::CreateTextureFromFile(crate_texture,
										"data/crate.png")) {
		return;
	}
	Texture planks_texture;
	if (!Utility::CreateTextureFromFile(planks_texture,
										"data/planks.png")) {
		return;
	}
	Texture marble_texture;
	if (!Utility::CreateTextureFromFile(marble_texture,
										"data/marble.png")) {
		return;
	}
	Texture bricks_texture;
	if (!Utility::CreateTextureFromFile(bricks_texture,
										"data/bricks.png")) {
		return;
	}
	SamplerState linear_sampler;
	if (!linear_sampler.Create(SAMPLER_FILTER_MODE_LINEAR,
							   SAMPLER_ADDRESS_MODE_WRAP,
							   SAMPLER_ADDRESS_MODE_WRAP)) {
		return;
	}
	const int cube_primitive_count = sizeof(cube_data) / sizeof(cube_data[0]);
	VertexBuffer cube;
	if (!cube.Create(sizeof(cube_data), cube_data)) {
		return;
	}
	const int floor_primitive_count = sizeof(floor_data) / sizeof(floor_data[0]);
	VertexBuffer floor;
	if (!floor.Create(sizeof(floor_data), floor_data)) {
		return;
	}
	VertexLayout layout;
	layout.AddAttribute(0, VertexLayout::ATTRIBUTE_FORMAT_FLOAT, 3, false);
	layout.AddAttribute(1, VertexLayout::ATTRIBUTE_FORMAT_FLOAT, 4, false);
	layout.AddAttribute(2, VertexLayout::ATTRIBUTE_FORMAT_FLOAT, 2, false);
	layout.AddAttribute(3, VertexLayout::ATTRIBUTE_FORMAT_FLOAT, 3, false);
	ShaderProgram final_program;
	if (!Utility::CreateShaderProgramFromFiles(final_program,
											   "data/final.vs.glsl",
											   "data/final.fs.glsl")) {
		return;
	}
	VertexBuffer final_buffer;
	if (!final_buffer.Create(sizeof(final_data), final_data)) {
		return;
	}
	VertexLayout final_layout;
	final_layout.AddAttribute(0, VertexLayout::ATTRIBUTE_FORMAT_FLOAT, 4, false);

	glm::mat4 projection = glm::perspective(3.141592f * 0.25f, 16.0f / 9.0f, 1.0f, 100.0f);

	Utility::Camera camera(projection);
	Utility::Controller controller(camera);
	
	float cube_rotation = 0.0f;
	glm::vec3 crate_position = glm::vec3(0.0f, 0.0f, -8.0f);
	glm::vec3 marble_position = glm::vec3(-2.0f, -2.0f, -8.0f);
	glm::vec3 planks_position = glm::vec3(2.0f, -2.0f, -8.0f);
	glm::vec3 floor_position = glm::vec3(0.0f, -6.0f, -10.0f);
	glm::vec3 wall_position = glm::vec3(0.0f, 0.0f, -14.0f);
	glm::vec3 wall_position2 = glm::vec3(10.0f, 6.0f, 0.0f);
	glm::vec3 wall_position3 = glm::vec3(10.0f,-6.0f, 0.0f);

	while (running && context.PollEvents()) {
		// rendering goes here

		controller.update(keyboard, mouse, time.Deltatime());
		mouse.Update();
		keyboard.Update();

		cube_rotation += 0.01f;
		glm::mat4 world = glm::translate(glm::mat4(1.0f), crate_position)
			* glm::rotate(glm::mat4(1.0f), cube_rotation, glm::normalize(glm::vec3(1.0f, 1.0f, -1.0f)));
		glm::mat4 world2 = glm::translate(glm::mat4(1.0f), floor_position);
		glm::mat4 world3 = glm::translate(glm::mat4(1.0f), wall_position)
			* glm::rotate(glm::mat4(1.0f), 3.141592f * 0.5f, glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f)));
		glm::mat4 world4 = glm::rotate(glm::mat4(1.0f), -3.141592f * 0.5f, glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f)))
			* glm::rotate(glm::mat4(1.0f), 3.141592f * 0.5f, glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f)))
			* glm::translate(glm::mat4(1.0f), wall_position2);
		glm::mat4 world5= glm::rotate(glm::mat4(1.0f), -3.141592f * 0.5f, glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f)))
			* glm::rotate(glm::mat4(1.0f), 3.141592f * 0.5f, glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f)))
			* glm::translate(glm::mat4(1.0f), wall_position3);
		glm::mat4 world6 = glm::translate(glm::mat4(1.0f), marble_position);
		glm::mat4 world7 = glm::translate(glm::mat4(1.0f), planks_position);

		glm::mat4 orthographic = glm::ortho(0.0f,
											1920.0f,//float(rendertarget.width_),
											1080.0f,//float(rendertarget.height_),
											0.0f);
		backend.SetFramebuffer(rendertarget);
		backend.Clear(0.1f, 0.2, 0.3f, 1.0f);
		backend.SetShaderProgram(world_program);
		backend.SetShaderUniform(world_program,
							  UNIFORM_TYPE_MATRIX,
							  "u_projection",
							  1, glm::value_ptr(projection));
		backend.SetShaderUniform(world_program,
								 UNIFORM_TYPE_MATRIX,
								 "u_view",
								 1, glm::value_ptr(camera.m_view));
		backend.SetShaderUniform(world_program,
							  UNIFORM_TYPE_MATRIX,
							  "u_world",
							  1, glm::value_ptr(world));
		backend.SetVertexBuffer(cube);
		backend.SetVertexLayout(layout);
		backend.SetTexture(crate_texture);
		backend.SetSamplerState(linear_sampler);
		backend.SetBlendState(false);
		backend.SetDepthState(true, true);
		backend.SetRasterizerState(CULL_MODE_BACK, FRONT_FACE_CW);
		backend.Draw(PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, 0, cube_primitive_count);

		backend.SetShaderUniform(world_program,
								 UNIFORM_TYPE_MATRIX,
								 "u_world",
								 1, glm::value_ptr(world6));
		backend.SetTexture(marble_texture);
		backend.Draw(PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, 0, cube_primitive_count);

		backend.SetShaderUniform(world_program,
								 UNIFORM_TYPE_MATRIX,
								 "u_world",
								 1, glm::value_ptr(world7));
		backend.SetTexture(planks_texture);
		backend.Draw(PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, 0, cube_primitive_count);

		backend.SetShaderUniform(world_program,
								 UNIFORM_TYPE_MATRIX,
								 "u_world",
								 1, glm::value_ptr(world2));
		backend.SetVertexBuffer(floor);
		backend.SetVertexLayout(layout);
		backend.SetTexture(grass_texture);
		backend.Draw(PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, 0, floor_primitive_count);

		backend.SetShaderUniform(world_program,
								 UNIFORM_TYPE_MATRIX,
								 "u_world",
								 1, glm::value_ptr(world3));
		backend.SetVertexBuffer(floor);
		backend.SetVertexLayout(layout);
		backend.SetTexture(bricks_texture);
		backend.Draw(PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, 0, floor_primitive_count);
		backend.SetShaderUniform(world_program,
								 UNIFORM_TYPE_MATRIX,
								 "u_world",
								 1, glm::value_ptr(world4));
		backend.Draw(PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, 0, floor_primitive_count);
		backend.SetShaderUniform(world_program,
								 UNIFORM_TYPE_MATRIX,
								 "u_world",
								 1, glm::value_ptr(world5));
		backend.Draw(PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, 0, floor_primitive_count);

		backend.ResetFramebuffer();
		backend.Clear(0.0f, 0.0f, 0.0f, 1.0f);
		backend.SetViewport(0, 0, 1920, 1080);
		backend.SetShaderProgram(final_program);
		backend.SetVertexBuffer(final_buffer);
		backend.SetVertexLayout(final_layout);
		backend.SetTexture(rendertarget.ColorAttachmentAsTexture(0));
		backend.SetSamplerState(linear_sampler);
		backend.SetBlendState(false);
		backend.SetRasterizerState(CULL_MODE_NONE, FRONT_FACE_CW);
		backend.Draw(PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, 0, 6);
		context.SwapBuffers();
	}
}

void Application::OnKey(int key, bool state) {
	if (key == GLFW_KEY_ESCAPE && !state) {
		running = false;
	}
	keyboard.OnKey(key, state);
}

void Application::OnMouse(int x, int y) {
	mouse.OnMove(x, y);
}

void Application::OnScroll(int xoffset, int yoffset) {
}

void Application::OnButton(int button, bool state) {
	mouse.OnButton(button, state);
}