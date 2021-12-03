// Utility.cpp

#include "Utility.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <cstdio>
#include <string>
#include <chrono>

template <typename F>
struct Defer {
	F f;
	Defer(F&& _f) : f(_f) {}
	~Defer() { f(); }
};

namespace Utility {
	bool CreateShaderProgramFromFiles(ShaderProgram& program,
		const char* vertex_filename,
		const char* fragment_filename) {
		std::size_t size = 0;
		FILE* file_in = nullptr;

		fopen_s(&file_in, vertex_filename, "r");
		if (file_in == nullptr) {
			return false;
		}

		fseek(file_in, 0, SEEK_END);
		size = ftell(file_in);
		fseek(file_in, 0, SEEK_SET);
		std::string vertex_source;
		vertex_source.resize(size + 1);
		fread(vertex_source.data(), 1, size, file_in);
		fclose(file_in);
		file_in = nullptr;

		fopen_s(&file_in, fragment_filename, "r");
		if (file_in == nullptr) {
			return false;
		}

		fseek(file_in, 0, SEEK_END);
		size = ftell(file_in);
		fseek(file_in, 0, SEEK_SET);
		std::string fragment_source;
		fragment_source.resize(size + 1);
		fread(fragment_source.data(), 1, size, file_in);
		fclose(file_in);
		file_in = nullptr;

		return program.Create(vertex_source.c_str(), fragment_source.c_str());
	}

	bool CreateTextureFromFile(Texture& texture,
		const char* filename) {
		int width = 0, height = 0, components = 0;
		auto data = stbi_load(filename, &width, &height, &components, STBI_default);
		assert(data);
		assert(components == 3 || components == 4);

		Defer release([&]() {
			stbi_image_free(data);
		});

		TextureFormat format = components == 3 ? TEXTURE_FORMAT_RGB8 : TEXTURE_FORMAT_RGBA8;
		return texture.Create(format, width, height, data);
	}

	int64 GetCurrentTick() {
		static int64 start = 0;
		if (start == 0) {
			auto ns = std::chrono::high_resolution_clock::now();
			auto ms = std::chrono::time_point_cast<std::chrono::milliseconds>(ns);
			start = ms.time_since_epoch().count();
		}

		auto ns = std::chrono::high_resolution_clock::now();
		auto ms = std::chrono::time_point_cast<std::chrono::milliseconds>(ns);
		auto now = ms.time_since_epoch().count();
		return now - start;
	}
} // !Utility
