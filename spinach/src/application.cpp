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

struct Skybox {
	Skybox() = default;

	bool IsValid() const
	{
		return program.IsValid() &&
			cubemap.IsValid() &&
			sampler.IsValid() &&
			buffer.IsValid();
	}

	bool Create()
	{
		// note: create shader program
		if (!Utility::CreateShaderProgramFromFiles(program,
												   "data/skybox/shader.vs.glsl",
												   "data/skybox/shader.fs.glsl"))
		{
			return false;
		}

		// note: load images and create cubemap
		const char* filenames[] =
		{
		   "data/skybox/xpos.jpg",
		   "data/skybox/xneg.jpg",
		   "data/skybox/ypos.jpg",
		   "data/skybox/yneg.jpg",
		   "data/skybox/zpos.jpg",
		   "data/skybox/zneg.jpg",
		};

		if (!Utility::CreateCubemapFromFiles(cubemap, 6, filenames)) {
			return false;
		}

		// note: create sampler state
		if (!sampler.Create(SAMPLER_FILTER_MODE_LINEAR,
							SAMPLER_ADDRESS_MODE_CLAMP,
							SAMPLER_ADDRESS_MODE_CLAMP))
		{
			return false;
		}


		{ // note: create vertex buffer and set vertex layout attributes
			const float Q = 2.0f;
			const glm::vec3 vertices[] =
			{
				// x positive
				{  Q,  Q, -Q },
				{  Q,  Q,  Q },
				{  Q, -Q,  Q },
				{  Q, -Q,  Q },
				{  Q, -Q, -Q },
				{  Q,  Q, -Q },

				// x negative
				{ -Q,  Q,  Q },
				{ -Q,  Q, -Q },
				{ -Q, -Q, -Q },
				{ -Q, -Q, -Q },
				{ -Q, -Q,  Q },
				{ -Q,  Q,  Q },

				// y positive
				{ -Q,  Q,  Q },
				{  Q,  Q,  Q },
				{  Q,  Q, -Q },
				{  Q,  Q, -Q },
				{ -Q,  Q, -Q },
				{ -Q,  Q,  Q },

				// y negative
				{ -Q, -Q, -Q },
				{  Q, -Q, -Q },
				{  Q, -Q,  Q },
				{  Q, -Q,  Q },
				{ -Q, -Q,  Q },
				{ -Q, -Q, -Q },

				// z positive
				{ -Q,  Q, -Q },
				{  Q,  Q, -Q },
				{  Q, -Q, -Q },
				{  Q, -Q, -Q },
				{ -Q, -Q, -Q },
				{ -Q,  Q, -Q },

				// z negative
				{  Q,  Q,  Q },
				{ -Q,  Q,  Q },
				{ -Q, -Q,  Q },
				{ -Q, -Q,  Q },
				{  Q, -Q,  Q },
				{  Q,  Q,  Q },
			};

			primitive_count = sizeof(vertices) / sizeof(vertices[0]);
			if (!buffer.Create(sizeof(vertices), vertices)) {
				return false;
			}

			layout.AddAttribute(0, VertexLayout::ATTRIBUTE_FORMAT_FLOAT, 3, false);
		}

		return true;
	}

	void Destroy()
	{
		program.Destroy();
		cubemap.Destroy();
		sampler.Destroy();
		buffer.Destroy();
	}

	void Draw(RenderBackend& backend, const Utility::Camera& camera)
	{
		glm::mat4 proj = camera.m_projection;
		glm::mat4 view = camera.m_view;
		view[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

		backend.SetShaderProgram(program);
		backend.SetShaderUniform(program, UNIFORM_TYPE_MATRIX, "u_projection", 1, glm::value_ptr(proj));
		backend.SetShaderUniform(program, UNIFORM_TYPE_MATRIX, "u_view", 1, glm::value_ptr(view));
		backend.SetVertexBuffer(buffer);
		backend.SetVertexLayout(layout);
		backend.SetCubemap(cubemap);
		backend.SetSamplerState(sampler);
		backend.SetBlendState(false);
		backend.SetDepthState(false, false);
		backend.SetRasterizerState(CULL_MODE_FRONT, FRONT_FACE_CCW, POLYGON_MODE_FILL);
		backend.Draw(PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, 0, primitive_count);
	}

	ShaderProgram program;
	Cubemap cubemap;
	SamplerState sampler;
	VertexBuffer buffer;
	VertexLayout layout;
	int32 primitive_count{};
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

	const FramebufferFormat shadow_buffer_formats[] = {
		FRAMEBUFFER_FORMAT_D32
	};
	Framebuffer shadow_buffer;
	if (!shadow_buffer.Create(1024, 1024, 1, shadow_buffer_formats)) {
		return;
	}
	const FramebufferFormat render_buffer_formats[] = {
		FRAMEBUFFER_FORMAT_RGBA8,
		FRAMEBUFFER_FORMAT_D32
	};
	Framebuffer render_buffer;
	if (!render_buffer.Create(1920, 1080, 2, render_buffer_formats)) {
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

	ShaderProgram shadow_sprite_program;
	if (!Utility::CreateShaderProgramFromFiles(shadow_sprite_program,
											   "data/shadow_sprite.vs.glsl",
											   "data/shadow_sprite.fs.glsl")) {
		return;
	}
	VertexBuffer shadow_sprite_buffer;
	if (!shadow_sprite_buffer.Create(sizeof(final_data), final_data)) {
		return;
	}
	VertexLayout shadow_sprite_layout;
	shadow_sprite_layout.AddAttribute(0, VertexLayout::ATTRIBUTE_FORMAT_FLOAT, 4, false);

	ShaderProgram shadow_program;
	if (!Utility::CreateShaderProgramFromFiles(shadow_program,
											   "data/shadow.vs.glsl",
											   "data/shadow.fs.glsl")) {
		return;
	}

	glm::mat4 projection = glm::perspective(3.141592f * 0.25f, 16.0f / 9.0f, 1.0f, 100.0f);

	Utility::Camera camera(projection);
	Utility::Controller controller(camera);
	Skybox skybox;
	if (!skybox.Create()) {
		return;
	}
	
	float cube_rotation = 0.0f, offset = 3.0f;
	glm::vec3 crate_position = glm::vec3(0.0f, 0.0f, -8.0f);
	glm::vec3 floor_position = glm::vec3(0.0f, -4.0f, -8.0f);
	glm::vec3 wall_position = glm::vec3(0.0f, 2.0f, -14.0f);

	enum class LightColor {
		White,
		Red,
		Green,
		Blue
	} light_color_enum = LightColor::White;


	glm::vec3 light_color = glm::vec3(1.0f, 0.95f, 0.9f);

	bool current = false;
	bool previous = false;

	while (running && context.PollEvents()) {
		// rendering goes here

		controller.update(keyboard, mouse, time.Deltatime());
		mouse.Update();
		keyboard.Update();

		cube_rotation += 0.01f;
		glm::mat4 world_crate = glm::translate(glm::mat4(1.0f), crate_position)
			* glm::rotate(glm::mat4(1.0f), cube_rotation, glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)))
			* glm::translate(glm::mat4(1.0f), glm::vec3(offset, 0.0f, 0.0f));
		glm::mat4 world_marble = glm::translate(glm::mat4(1.0f), crate_position)
			* glm::rotate(glm::mat4(1.0f), cube_rotation, glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)))
			* glm::translate(glm::mat4(1.0f), glm::vec3(-offset, 0.0f, 0.0f));
		glm::mat4 world_planks = glm::translate(glm::mat4(1.0f), crate_position)
			* glm::rotate(glm::mat4(1.0f), cube_rotation, glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)))
			* glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, offset));
		glm::mat4 world_floor = glm::translate(glm::mat4(1.0f), floor_position);
		glm::mat4 world_wall = glm::translate(glm::mat4(1.0f), wall_position)
			* glm::rotate(glm::mat4(1.0f), 3.141592f * 0.5f, glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f)));

		glm::mat4 orthographic = glm::ortho(0.0f,
											1920.0f,//float(rendertarget.width_),
											1080.0f,//float(rendertarget.height_),
											0.0f);
		glm::vec3 light_direction = glm::normalize(glm::vec3(-0.2f, -0.2f, 0.0f));
		glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0f), light_rotation_z, glm::vec3(0.0f, 0.0f, 1.0f));
		light_direction = glm::normalize(glm::vec3(rotation_matrix * glm::vec4(light_direction, 1.0f)));

		previous = current;
		current = mouse.ButtonDown(GLFW_MOUSE_BUTTON_RIGHT);
		
		if (current && !previous) {
			switch (light_color_enum) {
			case LightColor::White:
				light_color = glm::vec3(1.0f, 0.95f, 0.9f);
				light_color_enum = LightColor::Red;
				break;
			case LightColor::Red:
				light_color = glm::vec3(1.0f, 0.0f, 0.0f);
				light_color_enum = LightColor::Green;
				break;
			case LightColor::Green:
				light_color = glm::vec3(0.0f, 1.0f, 0.0f);
				light_color_enum = LightColor::Blue;
				break;
			case LightColor::Blue:
				light_color = glm::vec3(0.0f, 0.0f, 1.0f);
				light_color_enum = LightColor::White;
				break;
			}
		}

		glm::mat4 light_projection_matrix = glm::ortho<float>(-20.0f,
															  20.0f,
															  -20.0f,
															  20.0f,
															  -25.0f, 25.0f);
		glm::mat4 light_view_matrix = glm::lookAt(-light_direction,
												  glm::vec3(0.0f, 0.0f, 0.0f),
												  glm::vec3(0.0f, 1.0f, 0.0f));
		
		
		// Pass 1: Shadow Render Pass		
		backend.Clear(0.1f, 0.1f, 0.1f);
		backend.SetShaderProgram(shadow_program);
		backend.SetShaderUniform(shadow_program,
								 UNIFORM_TYPE_MATRIX,
								 "u_projection",
								 1, glm::value_ptr(light_projection_matrix));
		backend.SetShaderUniform(shadow_program,
								 UNIFORM_TYPE_MATRIX,
								 "u_view",
								 1, glm::value_ptr(light_view_matrix));

		backend.SetViewport(0, 0, 1024, 1024);
		backend.SetFramebuffer(shadow_buffer);
		glClear(GL_DEPTH_BUFFER_BIT);
		backend.SetTexture(crate_texture);

		backend.SetShaderUniform(shadow_program,
								 UNIFORM_TYPE_MATRIX,
								 "u_world",
								 1, glm::value_ptr(world_crate));
		backend.SetVertexBuffer(cube);
		backend.SetVertexLayout(layout);
		backend.SetSamplerState(linear_sampler);
		backend.SetBlendState(false);
		backend.SetDepthState(true, true);
		backend.SetRasterizerState(CULL_MODE_FRONT, FRONT_FACE_CW);
		backend.Draw(PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, 0, cube_primitive_count);

		backend.SetShaderUniform(shadow_program,
								 UNIFORM_TYPE_MATRIX,
								 "u_world",
								 1, glm::value_ptr(world_marble));
		backend.SetVertexBuffer(cube);
		backend.SetVertexLayout(layout);
		backend.SetSamplerState(linear_sampler);
		backend.SetBlendState(false);
		backend.SetDepthState(true, true);
		backend.SetRasterizerState(CULL_MODE_FRONT, FRONT_FACE_CW);
		backend.Draw(PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, 0, cube_primitive_count);

		backend.SetShaderUniform(shadow_program,
								 UNIFORM_TYPE_MATRIX,
								 "u_world",
								 1, glm::value_ptr(world_planks));
		backend.SetVertexBuffer(cube);
		backend.SetVertexLayout(layout);
		backend.SetSamplerState(linear_sampler);
		backend.SetBlendState(false);
		backend.SetDepthState(true, true);
		backend.SetRasterizerState(CULL_MODE_FRONT, FRONT_FACE_CW);
		backend.Draw(PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, 0, cube_primitive_count);

		backend.SetShaderUniform(shadow_program,
								 UNIFORM_TYPE_MATRIX,
								 "u_world",
								 1, glm::value_ptr(world_floor));
		backend.SetVertexBuffer(floor);
		backend.SetVertexLayout(layout);
		backend.SetSamplerState(linear_sampler);
		backend.SetBlendState(false);
		backend.SetDepthState(true, true);
		backend.SetRasterizerState(CULL_MODE_FRONT, FRONT_FACE_CW);
		backend.Draw(PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, 0, cube_primitive_count);

		backend.SetShaderUniform(shadow_program,
								 UNIFORM_TYPE_MATRIX,
								 "u_world",
								 1, glm::value_ptr(world_wall));
		backend.SetVertexBuffer(floor);
		backend.SetVertexLayout(layout);
		backend.SetSamplerState(linear_sampler);
		backend.SetBlendState(false);
		backend.SetDepthState(true, true);
		backend.SetRasterizerState(CULL_MODE_FRONT, FRONT_FACE_CW);
		backend.Draw(PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, 0, cube_primitive_count);

		backend.ResetFramebuffer();

		// Pass 2: Normal Render Pass
		backend.SetViewport(0, 0, 1920, 1080);
		backend.SetFramebuffer(render_buffer);
		backend.Clear(0.1f, 0.2, 0.3f);

		backend.SetShaderProgram(world_program);
		backend.SetSamplerState(linear_sampler, 1);
		backend.SetTexture(shadow_buffer.ColorAttachmentAsTexture(0), 1);

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
								 "u_light_projection",
								 1, glm::value_ptr(light_projection_matrix));
		backend.SetShaderUniform(world_program,
								 UNIFORM_TYPE_MATRIX,
								 "u_light_view",
								 1, glm::value_ptr(light_view_matrix));
		backend.SetShaderUniform(world_program,
								 UNIFORM_TYPE_VEC3,
								 "u_light_direction",
								 1, glm::value_ptr(light_direction));
		backend.SetShaderUniform(world_program,
								 UNIFORM_TYPE_VEC3,
								 "u_view_position",
								 1, glm::value_ptr(camera.m_position));
		backend.SetShaderUniform(world_program,
								 UNIFORM_TYPE_VEC3,
								 "u_light_color",
								 1, glm::value_ptr(light_color));
		backend.SetShaderUniform(world_program,
								 UNIFORM_TYPE_VEC3,
								 "u_light_ambient",
								 1, glm::value_ptr(glm::vec3(0.5f)));
		backend.SetShaderUniform(world_program,
								 UNIFORM_TYPE_VEC3,
								 "u_light_specular",
								 1, glm::value_ptr(glm::vec3(1.0f)));
		backend.SetShaderUniform(world_program,
								 UNIFORM_TYPE_VEC3,
								 "u_material_ambient",
								 1, glm::value_ptr(glm::vec3(0.5f)));
		backend.SetShaderUniform(world_program,
								 UNIFORM_TYPE_VEC3,
								 "u_material_specular",
								 1, glm::value_ptr(glm::vec3(1.0f)));

		backend.SetShaderUniform(world_program,
							  UNIFORM_TYPE_MATRIX,
							  "u_world",
							  1, glm::value_ptr(world_crate));
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
								 1, glm::value_ptr(world_marble));
		backend.SetVertexBuffer(cube);
		backend.SetVertexLayout(layout);
		backend.SetTexture(marble_texture);
		backend.SetSamplerState(linear_sampler);
		backend.SetBlendState(false);
		backend.SetDepthState(true, true);
		backend.SetRasterizerState(CULL_MODE_BACK, FRONT_FACE_CW);
		backend.Draw(PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, 0, cube_primitive_count);

		backend.SetShaderUniform(world_program,
								 UNIFORM_TYPE_MATRIX,
								 "u_world",
								 1, glm::value_ptr(world_planks));
		backend.SetVertexBuffer(cube);
		backend.SetVertexLayout(layout);
		backend.SetTexture(planks_texture);
		backend.SetSamplerState(linear_sampler);
		backend.SetBlendState(false);
		backend.SetDepthState(true, true);
		backend.SetRasterizerState(CULL_MODE_BACK, FRONT_FACE_CW);
		backend.Draw(PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, 0, cube_primitive_count);

		backend.SetShaderUniform(world_program,
								 UNIFORM_TYPE_MATRIX,
								 "u_world",
								 1, glm::value_ptr(world_floor));
		backend.SetVertexBuffer(floor);
		backend.SetVertexLayout(layout);
		backend.SetTexture(grass_texture);
		backend.Draw(PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, 0, floor_primitive_count);

		backend.SetShaderUniform(world_program,
								 UNIFORM_TYPE_MATRIX,
								 "u_world",
								 1, glm::value_ptr(world_wall));
		backend.SetVertexBuffer(floor);
		backend.SetVertexLayout(layout);
		backend.SetTexture(bricks_texture);
		backend.Draw(PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, 0, floor_primitive_count);
		
		backend.ResetFramebuffer();
		backend.SetViewport(0, 0, 1920, 1080);
		backend.Clear(0.0f, 0.0f, 0.0f, 1.0f);
		
		skybox.Draw(backend, camera);
		
		backend.SetShaderProgram(final_program);
		backend.SetVertexBuffer(final_buffer);
		backend.SetVertexLayout(final_layout);
		backend.SetSamplerState(linear_sampler);
		backend.SetTexture(render_buffer.ColorAttachmentAsTexture(0));
		backend.SetBlendState(false);
		backend.SetDepthState(false, false);
		backend.SetRasterizerState(CULL_MODE_NONE, FRONT_FACE_CW);
		backend.Draw(PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, 0, 6);

		backend.SetViewport(0, 760, 320, 320);
		backend.SetShaderProgram(shadow_sprite_program);
		backend.SetVertexLayout(shadow_sprite_layout);
		backend.SetVertexBuffer(shadow_sprite_buffer);
		backend.SetSamplerState(linear_sampler);
		backend.SetTexture(shadow_buffer.ColorAttachmentAsTexture(0));
		backend.SetBlendState(false);
		backend.SetDepthState(false, false);
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
	light_rotation_z += float(yoffset) * 0.1f;
	if (light_rotation_z > 360.0f)
		light_rotation_z = 0.0f;
	else if (light_rotation_z < 0.0f)
		light_rotation_z = 360.0f;
}

void Application::OnButton(int button, bool state) {
	mouse.OnButton(button, state);
}