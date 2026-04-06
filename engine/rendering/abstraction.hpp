#pragma once

#include <format>
#include <iterator>
#include <string>
#include "SDL_error.h"
#include "SDL_messagebox.h"
#include "SDL_video.h"
#include "rendering/mesh.hpp"
#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <core/node.hpp>
#include <fstream>
#include <vector>
#include <math/vec3.hpp>
#include <cerrno>
#include <cstring>
#include <core/camera.hpp>
#include <math/mat4.hpp>
#include <iostream>

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

		if (!gladLoadGLES2Loader((GLADloadproc)SDL_GL_GetProcAddress)) {
			SDL_ShowSimpleMessageBox(
				SDL_MESSAGEBOX_ERROR,
				"Error",
				"Failed to load glad with GLES2 loader.",
				nullptr
			);
			return false;
		}

		SDL_GL_SetSwapInterval(1); // vsync
		//glEnable(GL_DEPTH_TEST);
		//glDepthFunc(GL_LESS);

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

			return false;
		}

		glUseProgram(program);
		glViewport(0, 0, width, height);

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

	void render(const RenderableNode& renderable, Camera& camera) {
		Mesh* mesh = renderable.mesh;

		Transform model_transform = renderable.transform;
		Transform cam_transform = camera.transform;
		
		Mat4 model = Mat4::translate(model_transform.get_position()) *
					 Mat4::rotate(model_transform.get_orientation());

		Mat4 view = Mat4::rotate(cam_transform.get_orientation().conjugate()) *
					Mat4::translate(-cam_transform.get_position());

		Mat4 proj = Mat4::perspective(camera.fov, camera.aspect(), camera.near, camera.far);
		Mat4 mvp = proj * view * model;

		glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);

		GLuint posAttrib = static_cast<GLuint>(glGetAttribLocation(program, "a_pos"));
		std::cout << posAttrib << std::endl;
		glEnableVertexAttribArray(posAttrib);
		glVertexAttribPointer(
			posAttrib,
			3,
			GL_FLOAT,
			GL_FALSE,
			sizeof(Vec3),
			(void*)0
		);

		GLint loc = glGetUniformLocation(program, "u_mvp");
		glUniformMatrix4fv(loc, 1, GL_FALSE, mvp.m);

		glDrawElements(
			GL_TRIANGLES,
			static_cast<GLsizei>(mesh->indices.size()),
			GL_UNSIGNED_SHORT,
			0
		);

		glDisableVertexAttribArray(posAttrib);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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
