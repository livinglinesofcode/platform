#pragma once

#include <format>
#include <iterator>
#include <string>
#include "SDL_error.h"
#include "SDL_messagebox.h"
#include "rendering/mesh.hpp"
#include <SDL2/SDL.h>
#include <GLES2/gl2.h>
#include <core/node.hpp>
#include <fstream>
#include <vector>
#include <math/vec3.hpp>
#include <cstdint>
#include <cerrno>
#include <cstring>

struct RenderingContext {
	SDL_Window* window = nullptr;
	SDL_GLContext context = nullptr;
	GLuint program;

	bool create_window(const char* title, int width, int height) {
		if (SDL_Init(SDL_INIT_VIDEO) != 0) { 
			SDL_ShowSimpleMessageBox(
				SDL_MESSAGEBOX_ERROR,
				"Error",
				std::format("SDL initialization error: {}", SDL_GetError()).c_str(),
				nullptr
			);
			return false;
		};

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

		window = SDL_CreateWindow(title,
								  SDL_WINDOWPOS_CENTERED,
								  SDL_WINDOWPOS_CENTERED,
								  width,
								  height,
								  SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
		if (!window) {
			SDL_ShowSimpleMessageBox(
				SDL_MESSAGEBOX_ERROR,
				"Error",
				std::format("SDL window creation error: {}", SDL_GetError()).c_str(),
				nullptr
			);
			return false;
		}

		context = SDL_GL_CreateContext(window);
		if (!context) {
			SDL_ShowSimpleMessageBox(
				SDL_MESSAGEBOX_ERROR,
				"Error",
				std::format("SDL GL context creation error: {}", SDL_GetError()).c_str(),
				nullptr
			);
			return false;
		}

		SDL_GL_SetSwapInterval(1); // vsync
		
		GLuint vshader = compile_shader("shaders/vertex.vert", GL_VERTEX_SHADER);
		if (vshader == 0) return false;
		GLuint fshader = compile_shader("shaders/fragment.frag", GL_FRAGMENT_SHADER);
		if (fshader == 0) return false;

		program = glCreateProgram();
		glAttachShader(program, vshader);
		glAttachShader(program, fshader);
		glLinkProgram(program);

		GLint program_linked;
		glGetProgramiv(program, GL_LINK_STATUS, &program_linked);
		if (!program_linked) {
			char info[512];
			glGetProgramInfoLog(program, 512, nullptr, info);
			std::string msg = std::string("Error linking program after attaching shaders: ") + info;
			SDL_ShowSimpleMessageBox(
				SDL_MESSAGEBOX_ERROR,
				"Error",
				msg.c_str(),
				nullptr
			);
		}

		return true;
	}

	GLuint compile_shader(std::string path, GLenum type) {
		std::ifstream file(path);
		if (!file.is_open()) {
			std::string msg = "Failed to open shader file: " + path + "\nReason: " + std::strerror(errno);
			SDL_ShowSimpleMessageBox(
				SDL_MESSAGEBOX_ERROR,
				"Error",
				msg.c_str(),
				nullptr
			);

			return 0;
		}

		std::string source((std::istreambuf_iterator<char>(file)),
						   std::istreambuf_iterator<char>());

		GLuint shader = glCreateShader(type);
		const char* src = source.c_str();
		glShaderSource(shader, 1, &src, nullptr);
		glCompileShader(shader);

		GLint success;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			char info[512];
			glGetShaderInfoLog(shader, 512, nullptr, info);
			std::string msg = "Error compiling shader " + path + ": " + info;
			SDL_ShowSimpleMessageBox(
				SDL_MESSAGEBOX_ERROR,
				"Error",
				msg.c_str(),
				nullptr
			);

			return 0;
		}

		return shader;
	}

	void render(const RenderableNode& renderable) {
		char* vbo;

		Mesh* mesh = renderable.mesh;
		std::vector<Vec3> verts = mesh->vertices;
		std::vector<uint32_t> indices = mesh->indices;
	}

	void swap_buffers() {
		if (window) SDL_GL_SwapWindow(window);
	}

	void destroy() {
		if (context) SDL_GL_DeleteContext(context);
		if (window) SDL_DestroyWindow(window);
		SDL_Quit();
	}
};
