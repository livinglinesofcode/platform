#pragma once

#include <format>
#include <string>
#include "SDL_error.h"
#include "SDL_messagebox.h"
#include <SDL2/SDL.h>
#include <GLES2/gl2.h>

struct RenderingContext {
	SDL_Window* window = nullptr;
	SDL_GLContext context = nullptr;
	
	bool createWindow(const char* title, int width, int height) {
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
				window
			);
			return false;
		}

		SDL_GL_SetSwapInterval(1); // vsync
		return true;
	}

	void swapBuffers() {
		if (window) SDL_GL_SwapWindow(window);
	}

	void destroy() {
		if (context) SDL_GL_DeleteContext(context);
		if (window) SDL_DestroyWindow(window);
		SDL_Quit();
	}
};
