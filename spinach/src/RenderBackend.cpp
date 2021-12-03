// render_backend.cpp

#include "Render.h"

#include <cassert>
#include <glad/glad.h>

template<class T, size_t N>
constexpr size_t array_size(T(&)[N])
{
	return N;
}

static const GLenum gl_uniform_type[] =
{
   GL_FLOAT,
   GL_FLOAT_VEC2,
   GL_FLOAT_VEC3,
   GL_FLOAT_VEC4,
   GL_INT,
   GL_BOOL,
   GL_SAMPLER_2D,
   GL_FLOAT_MAT4,
};

static const GLuint gl_uniform_size[] =
{
   sizeof(float),
   sizeof(float) * 2,
   sizeof(float) * 3,
   sizeof(float) * 4,
   sizeof(int),
   sizeof(int),
   sizeof(int),
   sizeof(float) * 16,
};

static const GLenum gl_texture_format_internal[] =
{
   GL_RGB8,
   GL_RGBA8,
};

static const GLenum gl_texture_format[] =
{
   GL_RGB,
   GL_RGBA,
};

static const GLenum gl_texture_format_type[] =
{
   GL_UNSIGNED_BYTE,
   GL_UNSIGNED_BYTE,
};

static const GLenum gl_sampler_filter[] =
{
   GL_NEAREST,
   GL_LINEAR,
   GL_NEAREST_MIPMAP_NEAREST,
   GL_NEAREST_MIPMAP_LINEAR,
   GL_LINEAR_MIPMAP_NEAREST,
   GL_LINEAR_MIPMAP_LINEAR,
};

static const GLenum gl_sampler_address[] =
{
   GL_CLAMP_TO_EDGE,
   GL_REPEAT,
   GL_MIRRORED_REPEAT
};

const GLenum gl_buffer_usage_hint[] =
{
   GL_STATIC_DRAW,
   GL_STREAM_DRAW,
};

static const GLenum gl_blend_eq[] =
{
   GL_FUNC_ADD,
   GL_FUNC_SUBTRACT,
   GL_FUNC_REVERSE_SUBTRACT,
   GL_MIN,
   GL_MAX,
};

static const GLenum gl_blend_ft[] =
{
   GL_ZERO,
   GL_ONE,
   GL_SRC_COLOR,
   GL_ONE_MINUS_SRC_COLOR,
   GL_DST_COLOR,
   GL_ONE_MINUS_DST_COLOR,
   GL_SRC_ALPHA,
   GL_ONE_MINUS_SRC_ALPHA,
   GL_DST_ALPHA,
   GL_ONE_MINUS_DST_ALPHA,
   GL_CONSTANT_COLOR,
   GL_ONE_MINUS_CONSTANT_COLOR,
   GL_CONSTANT_ALPHA,
   GL_ONE_MINUS_CONSTANT_ALPHA,
   GL_SRC_ALPHA_SATURATE,
};

static const GLenum gl_compare_func[] =
{
   GL_NEVER,
   GL_LESS,
   GL_EQUAL,
   GL_LEQUAL,
   GL_GREATER,
   GL_NOTEQUAL,
   GL_GEQUAL,
   GL_ALWAYS,
};

static const GLenum gl_stencil_op[] =
{
   GL_KEEP,
   GL_ZERO,
   GL_REPLACE,
   GL_INCR,
   GL_INCR_WRAP,
   GL_DECR,
   GL_DECR_WRAP,
   GL_INVERT,
};

static const GLenum gl_cull_mode[] =
{
   GL_NONE,
   GL_BACK,
   GL_FRONT,
   GL_FRONT_AND_BACK,
};

static const GLenum gl_front_face[] =
{
   GL_CCW,
   GL_CW,
};

static const GLenum gl_primitive_topology[] =
{
   GL_POINTS,
   GL_LINES,
   GL_TRIANGLES,
};

static const GLenum gl_index_type[] =
{
   GL_UNSIGNED_BYTE,
   GL_UNSIGNED_SHORT,
   GL_UNSIGNED_INT,
};

static const GLuint gl_index_size[] =
{
   sizeof(uint8),
   sizeof(uint16),
   sizeof(uint32),
};

const GLenum gl_framebuffer_format_internal[] =
{
   GL_NONE,
   GL_RGB8,
   GL_RGBA8,
   GL_DEPTH24_STENCIL8,
};

const GLenum gl_framebuffer_format[] =
{
   GL_NONE,
   GL_RGB,
   GL_RGBA,
   GL_DEPTH_STENCIL,
};

static GLenum gl_framebuffer_type[] =
{
   GL_NONE,
   GL_UNSIGNED_BYTE,
   GL_UNSIGNED_BYTE,
   GL_UNSIGNED_INT_24_8,
};

static const GLenum gl_attribute_type[] =
{
   GL_FLOAT,
   GL_UNSIGNED_BYTE,
};

static const GLuint gl_attribute_size[] =
{
   sizeof(float),
   sizeof(char),
};

ShaderProgram::ShaderProgram()
	: id(0)
{
}

bool ShaderProgram::IsValid() const
{
	return id != 0;
}

bool ShaderProgram::Create(const char* vertex_shader_source,
						   const char* fragment_shader_source)
{
	GLuint vid = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vid, 1, &vertex_shader_source, nullptr);
	glCompileShader(vid);

	GLuint fid = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fid, 1, &fragment_shader_source, NULL);
	glCompileShader(fid);

	GLuint pid = glCreateProgram();
	glAttachShader(pid, vid);
	glAttachShader(pid, fid);
	glLinkProgram(pid);

	GLint link_status = 0;
	glGetProgramiv(pid, GL_LINK_STATUS, &link_status);
	if (link_status == GL_FALSE) {
		GLchar vertex_error[1024]{};
		glGetShaderInfoLog(fid, sizeof(vertex_error), NULL, vertex_error);

		GLchar fragment_error[1024]{};
		glGetShaderInfoLog(fid, sizeof(fragment_error), NULL, fragment_error);

		GLchar program_error[1024];
		glGetProgramInfoLog(pid, sizeof(program_error), NULL, program_error);
		//debug::error_box("ERROR!", "[program-log]:\n%s", program_error);
		assert(!"shader program error");

		glDetachShader(pid, vid);
		glDetachShader(pid, fid);
		glDeleteProgram(pid);
	}
	else {
		id = pid;
	}

	glDeleteShader(vid);
	glDeleteShader(fid);

	// note: setting the sampler uniform locations in the order
	//       the sampler uniforms are defined in the shaders 
	glUseProgram(pid);
	GLint sampler_count = 0;
	GLint active_uniform_count = 0;
	glGetProgramiv(pid, GL_ACTIVE_UNIFORMS, &active_uniform_count);
	for (int32 index = 0; index < active_uniform_count; index++) {
		GLint size = 0;
		GLenum type;
		GLchar name[128] = {};
		glGetActiveUniform(pid, index, sizeof(name), nullptr, &size, &type, name);
		if (type == GL_SAMPLER_2D ||
			type == GL_SAMPLER_CUBE) {
			GLint location = glGetUniformLocation(pid, name);
			if (location == -1) {
				continue;
			}

			glUniform1i(location, sampler_count);
			sampler_count++;
		}
	}

	return IsValid();
}

void ShaderProgram::Destroy()
{
	glDeleteProgram(id);
	id = 0;
}

Texture::Texture()
	: id(0)
{
}

bool Texture::IsValid() const
{
	return id != 0;
}

bool Texture::Create(const TextureFormat format,
					 const int32 width,
					 const int32 height,
					 const void* data)
{
	GLuint _id = 0;
	glGenTextures(1, &_id);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _id);
	glTexImage2D(GL_TEXTURE_2D,
				 0, // mip level
				 gl_texture_format_internal[format],
				 width,
				 height,
				 0,
				 gl_texture_format[format],
				 gl_texture_format_type[format],
				 data);
	glBindTexture(GL_TEXTURE_2D, 0);

	id = _id;

	return IsValid();
}

bool Texture::Create(const TextureFormat format,
					 const int32 width,
					 const int32 height,
					 const int32 count,
					 const void** data)
{
	GLuint _id = 0;
	glGenTextures(1, &_id);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _id);
	for (int32 index = 0; index < count; index++) {
		glTexImage2D(GL_TEXTURE_2D,
					 index, // mip level
					 gl_texture_format_internal[format],
					 (width >> index),
					 (height >> index),
					 0,
					 gl_texture_format[format],
					 gl_texture_format_type[format],
					 data[index]);
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	id = _id;

	return IsValid();
}

void Texture::Update(const TextureFormat format,
					 const int32 width,
					 const int32 height,
					 const void* data)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D,
				 0,
				 gl_texture_format_internal[format],
				 width,
				 height,
				 0,
				 gl_texture_format[format],
				 GL_UNSIGNED_BYTE,
				 data);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Destroy()
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &id);
	id = 0;
}

Cubemap::Cubemap()
	: id(0)
{
}

bool Cubemap::IsValid() const
{
	return id != 0;
}

bool Cubemap::Create(const TextureFormat format,
					 const int32 width,
					 const int32 height,
					 const void* data[6])
{
	GLuint _id = 0;
	glGenTextures(1, &_id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _id);
	for (int32 index = 0; index < 6; index++) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + index,
					 0,
					 gl_texture_format_internal[format],
					 width,
					 height,
					 0,
					 gl_texture_format[format],
					 GL_UNSIGNED_BYTE,
					 data[index]);
	}

	id = _id;

	return IsValid();
}

void Cubemap::Destroy()
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &id);
	id = 0;
}

VertexBuffer::VertexBuffer()
	: id(0)
{
}

bool VertexBuffer::IsValid() const
{
	return id != 0;
}

bool VertexBuffer::Create(const int32 size,
						  const void* data,
						  const BufferUsageHint hint)
{
	GLuint _id = 0;
	glGenBuffers(1, &_id);
	glBindBuffer(GL_ARRAY_BUFFER, _id);
	glBufferData(GL_ARRAY_BUFFER, size, data, gl_buffer_usage_hint[hint]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	id = _id;

	return IsValid();
}

void VertexBuffer::Update(const int32 size,
						  const void* data)
{
	glBindBuffer(GL_ARRAY_BUFFER, id);
	//glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STREAM_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::Destroy()
{
	glDeleteBuffers(1, &id);
	id = 0;
}

IndexBuffer::IndexBuffer()
	: id(0)
{
}

bool IndexBuffer::IsValid() const
{
	return id != 0;
}

bool IndexBuffer::Create(const int32 size,
						 const void* data)
{
	GLuint _id = 0;
	glGenBuffers(1, &_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	id = _id;

	return IsValid();
}

void IndexBuffer::Destroy()
{
	glDeleteBuffers(1, &id);
	id = 0;
}

SamplerState::SamplerState()
	: id(0)
{
}

bool SamplerState::IsValid() const
{
	return id != 0;
}

bool SamplerState::Create(const SamplerFilterMode filter,
						  const SamplerAddressMode addr_u,
						  const SamplerAddressMode addr_v,
						  const SamplerAddressMode addr_w)
{
	GLuint _id = 0;
	glGenSamplers(1, &_id);
	//glBindSampler(0, id); // note: needed?
	glSamplerParameteri(_id, GL_TEXTURE_MIN_FILTER, gl_sampler_filter[filter]);
	glSamplerParameteri(_id, GL_TEXTURE_MAG_FILTER, filter == SAMPLER_FILTER_MODE_NEAREST ? GL_NEAREST : GL_LINEAR);
	glSamplerParameteri(_id, GL_TEXTURE_WRAP_S, gl_sampler_address[addr_u]);
	glSamplerParameteri(_id, GL_TEXTURE_WRAP_T, gl_sampler_address[addr_v]);
	glSamplerParameteri(_id, GL_TEXTURE_WRAP_R, gl_sampler_address[addr_w]);

	id = _id;

	return IsValid();
}

void SamplerState::Destroy()
{
	glDeleteSamplers(1, &id);
	id = 0;
}

Framebuffer::Framebuffer()
	: id(0)
	, width(0)
	, height(0)
	, depth_attachment{}
	, color_attachments{}
{
}

bool Framebuffer::IsValid() const
{
	return id != 0;
}

bool Framebuffer::Create(const int32 _width,
						 const int32 _height,
						 const int32 color_attachment_format_count,
						 const FramebufferFormat* color_attachment_formats,
						 const bool has_depth_attachment,
						 const FramebufferFormat depth_attachment_format)
{
	assert(_width > 0);
	assert(_height > 0);
	assert(color_attachment_format_count > 0);
	assert(color_attachment_format_count < Framebuffer::ATTACHMENT_LIMIT);

	GLuint _id = 0;
	glGenFramebuffers(1, &_id);
	glBindFramebuffer(GL_FRAMEBUFFER, _id);

	GLuint textures[Framebuffer::ATTACHMENT_LIMIT] = {};
	glGenTextures(color_attachment_format_count, textures);

	int32 color_attachment_count = 0;
	int32 depth_attachment_count = 0;

	for (int32 attachment_index = 0;
		 attachment_index < color_attachment_format_count;
		 attachment_index++)
	{
		const FramebufferFormat format = color_attachment_formats[attachment_index];

		glBindTexture(GL_TEXTURE_2D, textures[attachment_index]);
		glTexImage2D(GL_TEXTURE_2D,
					 0,
					 gl_framebuffer_format_internal[format],
					 _width,
					 _height,
					 0,
					 gl_framebuffer_format[format],
					 gl_framebuffer_type[format],
					 NULL);

		if (format == FRAMEBUFFER_FORMAT_D32) {
			assert(depth_attachment_count < 1);
			glFramebufferTexture2D(GL_FRAMEBUFFER,
								   GL_DEPTH_STENCIL_ATTACHMENT,
								   GL_TEXTURE_2D,
								   textures[attachment_index],
								   0);
			depth_attachment_count++;
		}
		else {
			glFramebufferTexture2D(GL_FRAMEBUFFER,
								   GL_COLOR_ATTACHMENT0 + color_attachment_count,
								   GL_TEXTURE_2D,
								   textures[attachment_index],
								   0);
			color_attachment_count++;
		}
	}

	GLuint rbo = 0;
	if (has_depth_attachment) {
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);

		glRenderbufferStorage(GL_RENDERBUFFER,
							  gl_framebuffer_format_internal[depth_attachment_format],
							  _width,
							  _height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER,
								  GL_DEPTH_STENCIL_ATTACHMENT,
								  GL_RENDERBUFFER,
								  rbo);
	}

	GLenum complete_status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (complete_status != GL_FRAMEBUFFER_COMPLETE) {
		assert(false);
	}

	width = _width;
	height = _height;
	id = _id;
	depth_attachment = rbo;
	for (int32 index = 0; index < Framebuffer::ATTACHMENT_LIMIT; index++) {
		color_attachments[index] = textures[index];
	}

	GLenum attachment_indices[Framebuffer::ATTACHMENT_LIMIT] = {
	   GL_COLOR_ATTACHMENT0 + 0,
	   GL_COLOR_ATTACHMENT0 + 1,
	   GL_COLOR_ATTACHMENT0 + 2,
	   GL_COLOR_ATTACHMENT0 + 3,
	};
	glDrawBuffers(color_attachment_count, attachment_indices);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return IsValid();
}

void Framebuffer::Destroy()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDeleteFramebuffers(1, &id);
	if (depth_attachment) {
		glDeleteRenderbuffers(1, &depth_attachment);
	}

	for (uint32 index = 0; index < Framebuffer::ATTACHMENT_LIMIT; index++) {
		if (color_attachments[index]) {
			glDeleteTextures(1, color_attachments + index);
			color_attachments[index] = 0;
		}
	}

	id = 0;
	width = 0;
	height = 0;
	depth_attachment = 0;
}

Texture Framebuffer::ColorAttachmentAsTexture(const int32 index) const
{
	assert(index >= 0 && index < Framebuffer::ATTACHMENT_LIMIT);
	Texture result;
	result.id = color_attachments[index];
	return result;
}

VertexLayout::VertexLayout()
	: stride(0)
	, attribute_count(0)
	, attributes{}
{
}

void VertexLayout::AddAttribute(const int32 index,
								AttributeFormat format,
								const int32 count,
								const bool normalized)
{
	assert(attribute_count < array_size(attributes));

	const int32 at = attribute_count++;
	attributes[at].command_index = index;
	attributes[at].format = format;
	attributes[at].count = count;
	attributes[at].offset = stride;
	attributes[at].normalized = normalized;

	stride += count * gl_attribute_size[format];
}

void VertexLayout::Clear()
{
	stride = 0;
	attribute_count = 0;
	for (int32 index = 0; index < 8; index++) {
		attributes[index] = {};
	}
}

// note: opengl core context _requires_ a vertex array object to be bound
//       so let's please the opengl gods
static GLuint g_vertex_array_object = 0;

RenderBackend::RenderBackend()
{
	if (g_vertex_array_object == 0) {
		glGenVertexArrays(1, &g_vertex_array_object);
		glBindVertexArray(g_vertex_array_object);
	}
}

RenderBackend::~RenderBackend()
{
	if (g_vertex_array_object) {
		glBindVertexArray(0);
		glDeleteVertexArrays(1, &g_vertex_array_object);
		g_vertex_array_object = 0;
	}
}

void RenderBackend::Clear(const float red,
						  const float green,
						  const float blue,
						  const float alpha,
						  const float depth)
{
	glDepthMask(GL_TRUE);
	glClearDepth(depth);
	glClearColor(red, green, blue, alpha);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderBackend::SetViewport(const int32 x,
								const int32 y,
								const int32 width,
								const int32 height)
{
	glViewport(x, y, width, height);
}

void RenderBackend::SetFramebuffer(const Framebuffer& handle)
{
	glBindFramebuffer(GL_FRAMEBUFFER, handle.id);
	SetViewport(0, 0, handle.width, handle.height);
}

void RenderBackend::ResetFramebuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderBackend::SetShaderProgram(const ShaderProgram& handle)
{
	glUseProgram(handle.id);
}

void RenderBackend::SetShaderUniform(const ShaderProgram& handle,
									 const UniformType type,
									 const char* name,
									 const int32 count,
									 const void* value)
{
	GLint location = glGetUniformLocation(handle.id, name);
	if (location == -1)
		return;

	switch (type) {
	case UNIFORM_TYPE_FLOAT:
	{
		glUniform1fv(location, count, (const GLfloat*)value);
	} break;
	case UNIFORM_TYPE_VEC2:
	{
		glUniform2fv(location, count, (const GLfloat*)value);
	} break;
	case UNIFORM_TYPE_VEC3:
	{
		glUniform3fv(location, count, (const GLfloat*)value);
	} break;
	case UNIFORM_TYPE_VEC4:
	{
		glUniform4fv(location, count, (const GLfloat*)value);
	} break;
	case UNIFORM_TYPE_INT:
	case UNIFORM_TYPE_BOOL:
	case UNIFORM_TYPE_SAMPLER:
	{
		glUniform1iv(location, count, (const GLint*)value);
	} break;
	case UNIFORM_TYPE_MATRIX:
	{
		glUniformMatrix4fv(location, count, GL_FALSE, (const GLfloat*)value);
	} break;
	}
}

void RenderBackend::SetIndexBuffer(const IndexBuffer& handle)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle.id);
}

void RenderBackend::SetVertexBuffer(const VertexBuffer& handle)
{
	glBindBuffer(GL_ARRAY_BUFFER, handle.id);
}

void RenderBackend::SetVertexLayout(const VertexLayout& layout)
{
	// note: disable all attributes
	for (int32 index = 0; index < array_size(layout.attributes); index++) {
		glDisableVertexAttribArray(index);
	}

	const int32 vertex_stride = layout.stride;
	const int32 attribute_count = layout.attribute_count;
	for (int32 attribute_index = 0;
		 attribute_index < attribute_count;
		 attribute_index++)
	{
		const auto& attribute = layout.attributes[attribute_index];
		glEnableVertexAttribArray(attribute.command_index);
		glVertexAttribPointer(attribute.command_index,
							  attribute.count,
							  gl_attribute_type[attribute.format],
							  attribute.normalized,
							  vertex_stride,
							  (const void*)(uintptr_t)attribute.offset);
	}
}

void RenderBackend::SetTexture(const Texture& handle,
							   const int32 unit)
{
	assert(unit >= 0 && unit < 4);
	const GLenum units[] =
	{
	   GL_TEXTURE0,
	   GL_TEXTURE1,
	   GL_TEXTURE2,
	   GL_TEXTURE3,
	};

	glActiveTexture(units[unit]);
	glBindTexture(GL_TEXTURE_2D, handle.id);
}

void RenderBackend::SetCubemap(const Cubemap& handle,
							   const int32 unit)
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, handle.id);
}

void RenderBackend::SetSamplerState(const SamplerState& handle,
									const int32 unit)
{
	glBindSampler(unit, handle.id);
}

void RenderBackend::SetBlendState(const bool enabled,
								  const BlendEquation eq_rgb,
								  const BlendFactor src_rgb,
								  const BlendFactor dst_rgb,
								  const BlendEquation eq_alpha,
								  const BlendFactor src_alpha,
								  const BlendFactor dst_alpha)
{
	if (enabled) {
		glEnable(GL_BLEND);
		glBlendFuncSeparate(gl_blend_ft[src_rgb],
							gl_blend_ft[dst_rgb],
							gl_blend_ft[src_alpha],
							gl_blend_ft[dst_alpha]);
		glBlendEquationSeparate(gl_blend_eq[eq_rgb],
								gl_blend_eq[eq_alpha]);
	}
	else {
		glDisable(GL_BLEND);
	}
}

void RenderBackend::SetDepthState(const bool testing,
								  const bool write,
								  const float range_near,
								  const float range_far,
								  const CompareFunc func)
{
	if (testing) {
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(gl_compare_func[func]);

		if (write)
			glDepthMask(GL_TRUE);
		else
			glDepthMask(GL_FALSE);
	}
	else {
		glDisable(GL_DEPTH_TEST);
	}

	glDepthRange(range_near, range_far);
}

void RenderBackend::SetRasterizerState(const CullMode cull_mode,
									   const FrontFace front_face,
									   const PolygonMode polygon_mode)
{
	if (cull_mode != CULL_MODE_NONE) {
		glEnable(GL_CULL_FACE);
		glCullFace(gl_cull_mode[cull_mode]);
	}
	else {
		glDisable(GL_CULL_FACE);
	}

	glFrontFace(gl_front_face[front_face]);
	glPolygonMode(GL_FRONT_AND_BACK, polygon_mode == POLYGON_MODE_FILL ? GL_FILL : GL_LINE);
}

void RenderBackend::Draw(const PrimitiveTopology topology,
						 const int32 start_index,
						 const int32 primitive_count)
{
	glDrawArrays(gl_primitive_topology[topology],
				 start_index,
				 primitive_count);
}

void RenderBackend::DrawIndexed(const PrimitiveTopology topology,
								const IndexType type,
								const int32 start_index,
								const int32 primitive_count)
{
	// todo: glDrawElementsBaseVertex
	glDrawElements(gl_primitive_topology[topology],
				   primitive_count,
				   gl_index_type[type],
				   (const void*)(uintptr_t)(gl_index_size[type] * start_index));
}
