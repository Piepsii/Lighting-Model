#pragma once

#include <Render.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Utility {
	int64 GetCurrentTick();
	bool CreateShaderProgramFromFiles(ShaderProgram& program,
		const char* vertex_filename,
		const char* fragment_filename);
	bool CreateTextureFromFile(Texture& texture,
		const char* filename);
}

