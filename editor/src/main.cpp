#include <rendering/abstraction.hpp>
#include <scripting/script_bindings.cpp>
#include <core/camera.hpp>

int main() {
	RenderingContext ctx;
	if (!ctx.createWindow("Editor", 800, 500)) return -1;

	Camera camera;

	bool running = true;
	SDL_Event e;
	while (running) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) running = false;
		}
			
		glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ctx.swapBuffers();
	}
	
	ctx.destroy();
	return 0;
}
