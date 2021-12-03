// Render.h

#pragma once

using uint64 = unsigned long long;
using int64 = signed long long;
using uint32 = unsigned int;
using  int32 = signed int;
using uint16 = unsigned short;
using  int16 = signed short;
using  uint8 = unsigned char;
using   int8 = signed char;

enum UniformType
{
	UNIFORM_TYPE_FLOAT,
	UNIFORM_TYPE_VEC2,
	UNIFORM_TYPE_VEC3,
	UNIFORM_TYPE_VEC4,
	UNIFORM_TYPE_INT,
	UNIFORM_TYPE_BOOL,
	UNIFORM_TYPE_SAMPLER,
	UNIFORM_TYPE_MATRIX,
};

enum TextureFormat
{
	TEXTURE_FORMAT_RGB8,
	TEXTURE_FORMAT_RGBA8,
	TEXTURE_FORMAT_COUNT,
	TEXTURE_FORMAT_UNKNOWN,
};

enum CubemapFace
{
	CUBEMAP_FACE_POSITIVE_X,
	CUBEMAP_FACE_NEGATIVE_X,
	CUBEMAP_FACE_POSITIVE_Y,
	CUBEMAP_FACE_NEGATIVE_Y,
	CUBEMAP_FACE_POSITIVE_Z,
	CUBEMAP_FACE_NEGATIVE_Z,
	CUBEMAP_FACE_COUNT,
};

enum SamplerFilterMode
{
	SAMPLER_FILTER_MODE_NEAREST,
	SAMPLER_FILTER_MODE_LINEAR,
	SAMPLER_FILTER_MODE_NEAREST_MIP_NEAREST,
	SAMPLER_FILTER_MODE_NEAREST_MIP_LINEAR,
	SAMPLER_FILTER_MODE_LINEAR_MIP_NEAREST,
	SAMPLER_FILTER_MODE_LINEAR_MIP_LINEAR,
};

enum SamplerAddressMode
{
	SAMPLER_ADDRESS_MODE_CLAMP,
	SAMPLER_ADDRESS_MODE_WRAP,
	SAMPLER_ADDRESS_MODE_MIRROR,
};

enum BufferUsageHint
{
	BUFFER_USAGE_HINT_STATIC,
	BUFFER_USAGE_HINT_DYNAMIC,
};

enum BlendEquation
{
	BLEND_EQUATION_ADD,
	BLEND_EQUATION_SUBTRACT,
	BLEND_EQUATION_REVERSE_SUBTRACT,
	BLEND_EQUATION_MIN,
	BLEND_EQUATION_MAX,
};

enum BlendFactor
{
	BLEND_FACTOR_ZERO,
	BLEND_FACTOR_ONE,
	BLEND_FACTOR_SRC_COLOR,
	BLEND_FACTOR_ONE_MINUS_SRC_COLOR,
	BLEND_FACTOR_DST_COLOR,
	BLEND_FACTOR_ONE_MINUS_DST_COLOR,
	BLEND_FACTOR_SRC_ALPHA,
	BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
	BLEND_FACTOR_DST_ALPHA,
	BLEND_FACTOR_ONE_MINUS_DST_ALPHA,
	BLEND_FACTOR_CONSTANT_COLOR,
	BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR,
	BLEND_FACTOR_CONSTANT_ALPHA,
	BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA,
	BLEND_FACTOR_SRC_ALPHA_SATURATE,
};

enum CompareFunc
{
	COMPARE_FUNC_NEVER,
	COMPARE_FUNC_LESS,
	COMPARE_FUNC_EQUAL,
	COMPARE_FUNC_LESS_EQUAL,
	COMPARE_FUNC_GREATER,
	COMPARE_FUNC_NOT_EQUAL,
	COMPARE_FUNC_GREATER_EQUAL,
	COMPARE_FUNC_ALWAYS,
};

enum StencilOp
{
	STENCIL_OP_KEEP,
	STENCIL_OP_ZERO,
	STENCIL_OP_REPLACE,
	STENCIL_OP_INCR,
	STENCIL_OP_INCR_WRAP,
	STENCIL_OP_DECR,
	STENCIL_OP_DECR_WRAP,
	STENCIL_OP_INVERT,
};

enum CullMode
{
	CULL_MODE_NONE,
	CULL_MODE_BACK,
	CULL_MODE_FRONT,
	CULL_MODE_BOTH,
};

enum FrontFace
{
	FRONT_FACE_CCW,
	FRONT_FACE_CW,
};

enum PolygonMode
{
	POLYGON_MODE_FILL,
	POLYGON_MODE_WIREFRAME,
};

enum PrimitiveTopology
{
	PRIMITIVE_TOPOLOGY_POINT_LIST,
	PRIMITIVE_TOPOLOGY_LINE_LIST,
	PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
};

enum IndexType
{
	INDEX_TYPE_UNSIGNED_BYTE,
	INDEX_TYPE_UNSIGNED_SHORT,
	INDEX_TYPE_UNSIGNED_INT,
};

enum FramebufferFormat
{
	FRAMEBUFFER_FORMAT_NONE,
	FRAMEBUFFER_FORMAT_RGB8,
	FRAMEBUFFER_FORMAT_RGBA8,
	FRAMEBUFFER_FORMAT_D32,
	FRAMEBUFFER_FORMAT_COUNT,
	FRAMEBUFFER_FORMAT_INVALID,
};

struct ShaderProgram {
	ShaderProgram();

	bool IsValid() const;
	bool Create(const char* vertex_shader_source,
		const char* fragment_shader_source);
	void Destroy();

	uint32 id;
};

struct Texture {
	Texture();
	
	bool IsValid() const;
	bool Create(const TextureFormat format,
		const int32 width,
		const int32 height,
		const void* data);
	bool Create(const TextureFormat format,
		const int32 width,
		const int32 height,
		const int32 count,
		const void** data);
	void Update(const TextureFormat format,
		const int32 width,
		const int32 height,
		const void* data);
	void Destroy();

	uint32 id;
};

struct Cubemap {
	Cubemap();

	bool IsValid() const;
	bool Create(const TextureFormat format,
		const int32 width,
		const int32 height,
		const void* data[6]);
	void Destroy();

	uint32 id;
};

struct VertexBuffer {
	VertexBuffer();

	bool IsValid() const;
	bool Create(const int32 size,
		const void* data,
		const BufferUsageHint hint = BUFFER_USAGE_HINT_STATIC);
	void Update(const int32 size,
		const void* data);
	void Destroy();

	uint32 id;
};

struct IndexBuffer {
	IndexBuffer();

	bool IsValid() const;
	bool Create(const int32 size,
		const void* data);
	void Destroy();

	uint32 id;
};

struct SamplerState {
	SamplerState();

	bool IsValid() const;
	bool Create(const SamplerFilterMode filter,
		const SamplerAddressMode address_u,
		const SamplerAddressMode address_v,
		const SamplerAddressMode address_w = SAMPLER_ADDRESS_MODE_CLAMP);
	void Destroy();

	uint32 id;
};

struct Framebuffer {
	static constexpr int32 ATTACHMENT_LIMIT = 4;

	Framebuffer();

	bool IsValid() const;
	bool Create(const int32 width,
		const int32 height,
		const int32 color_attachment_format_count,
		const FramebufferFormat* color_attachment_formats,
		const bool has_depth_attachment = false,
		const FramebufferFormat depth_attachment_format = FRAMEBUFFER_FORMAT_NONE);
	void Destroy();

	Texture ColorAttachmentAsTexture(const int32 index) const;

	uint32 id;
	int32 width;
	int32 height;
	uint32 depth_attachment;
	uint32 color_attachments[ATTACHMENT_LIMIT];
};

struct VertexLayout {
	enum AttributeFormat {
		ATTRIBUTE_FORMAT_FLOAT,
		ATTRIBUTE_FORMAT_BYTE
	};

	struct Attribute {
		int32 command_index;
		AttributeFormat format;
		int32 count;
		int32 offset;
		bool normalized;
	};

	VertexLayout();

	void AddAttribute(const int32 index,
		AttributeFormat format,
		const int32 count,
		const bool normalized);
	void Clear();

	int32 stride;
	int32 attribute_count;
	Attribute attributes[8];
};

struct RenderBackend {
	RenderBackend();
	~RenderBackend();

	void Clear(const float red,
		const float green,
		const float blue,
		const float alpha = 1.0f,
		const float depth = 1.0f);
	void SetViewport(const int32 x,
		const int32 y,
		const int32 width,
		const int32 height);
	void SetFramebuffer(const Framebuffer& handle);
	void ResetFramebuffer();
	void SetShaderProgram(const ShaderProgram& handle);
	void SetShaderUniform(const ShaderProgram& handle,
		const UniformType type,
		const char* name,
		const int32 count,
		const void* value);
	void SetIndexBuffer(const IndexBuffer& handle);
	void SetVertexBuffer(const VertexBuffer& handle);
	void SetVertexLayout(const VertexLayout& layout);
	void SetTexture(const Texture& handle,
		const int32 unit = 0);
	void SetCubemap(const Cubemap& handle,
		const int32 unit = 0);
	void SetSamplerState(const SamplerState& handle,
		const int32 unit = 0);
	void SetBlendState(const bool enabled,
		const BlendEquation eq_rgb = BLEND_EQUATION_ADD,
		const BlendFactor src_rgb = BLEND_FACTOR_SRC_ALPHA,
		const BlendFactor dst_rgb = BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
		const BlendEquation eq_alpha = BLEND_EQUATION_ADD,
		const BlendFactor src_alpha = BLEND_FACTOR_ONE,
		const BlendFactor dst_alpha = BLEND_FACTOR_ONE);
	void SetDepthState(const bool testing,
		const bool write,
		const float range_near = 0.0f,
		const float range_far = 1.0f,
		const CompareFunc func = COMPARE_FUNC_LESS);
	void SetRasterizerState(const CullMode cull_mode = CULL_MODE_NONE,
		const FrontFace front_face = FRONT_FACE_CCW,
		const PolygonMode polygon_mode = POLYGON_MODE_FILL);
	void Draw(const PrimitiveTopology topology,
		const int32 start_index,
		const int32 primitive_count);
	void DrawIndexed(const PrimitiveTopology topology,
		const IndexType type,
		const int32 start_index,
		const int32 primitive_count);
};
