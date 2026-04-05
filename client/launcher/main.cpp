#include <rendering/abstraction.hpp>

int main() {
	RenderingContext ctx;
	if (!ctx.create_window("Client", 800, 500)) return -1;

	bool running = true;
	SDL_Event e;
	while (running) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) running = false;
		}

		glClearColor(0.2, 0.2, 0.3, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		ctx.swap_buffers();
	}

	ctx.destroy();
	return 0;
}
