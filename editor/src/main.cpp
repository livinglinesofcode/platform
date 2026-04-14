#include "mesh.hpp"
#include <SDL_keyboard.h>
#include <SDL_mouse.h>
#include <SDL_timer.h>
#include <memory>
#include <rendering/abstraction.hpp>
#include <core/node.hpp>
#include <core/camera2d.hpp>
#include <math/vec2.hpp>
#include <sys/stat.h>
#include <math/utils.hpp>
#include <iostream>

struct Vertex2D {
	float x, y;
};

bool render_quad(float s, const Mat4& ortho, const RenderingContext& ctx) {
	s *= 0.5;

	Vertex2D verts[4] = {
		{-s,  s}, // top left
		{ s,  s}, // top right
		{ s, -s}, // bottom right
		{-s, -s}  // bottom left
	};

	unsigned int indices[6] = {
		0, 1, 2,
		0, 3, 2
	};

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

	GLint a_index;
	a_index = glGetAttribLocation(ctx.program, "a_pos");
	if (a_index == -1) {
		std::cerr << "a_pos vertex attribute not found" << std::endl;
		return false;
	}

	glEnableVertexAttribArray(a_index);
	glVertexAttribPointer(a_index, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*)0);

	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	GLint u_index;
	u_index = glGetUniformLocation(ctx.program, "u_ortho");
	if (u_index == -1) {
		std::cerr << "u_ortho uniform not found" << std::endl;
		return false;
	}

	glUniformMatrix4fv(u_index, 1, GL_FALSE, ortho.m);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	return true;
}

int main() {
	RenderingContext ctx;

	const int width  = 800;
	const int height = 500;
	if (!ctx.create_window("Editor", width, height)) return -1;

	std::unique_ptr<Node> root = std::make_unique<Node>();
	root->name = "Root";

	Camera2D* camera = static_cast<Camera2D*>(
		root->add_child(std::make_unique<Camera2D>())
	);
	camera->set_viewport_size(width, height);

	Uint32 last = SDL_GetTicks();
	int dx, dy;

	bool running = true;
	SDL_Event e;

	// UPDATE PIPELINE //
	while (running) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) running = false;
		}

		Uint32 now = SDL_GetTicks();
		float dt = (now - last) / 1000.0f; // seconds
		last = now;

		// INPUT //
		const Uint8* keys = SDL_GetKeyboardState(nullptr);
		Uint32 buttons = SDL_GetRelativeMouseState(&dx, &dy);

		// RENDERING //
		glClearColor(0.3f, 0.3f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		render_quad(10.0f, camera->get_ortho(), ctx);

		ctx.swap_buffers();
	}
	
	ctx.destroy();
	return 0;
}
