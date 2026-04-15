#include <rendering/abstraction.hpp>
#include <iostream>
#include <SDL_keyboard.h>
#include <SDL_mouse.h>
#include <SDL_timer.h>
#include <core/node.hpp>
#include <core/camera2d.hpp>
#include <math/vec2.hpp>
#include <rendering/mesh.hpp>
#include <sys/stat.h>
#include <math/utils.hpp>
#include <iostream>

int main() {
	RenderingContext ctx;

	const int width  = 800;
	const int height = 500;
	if (!ctx.create_window("Editor", width, height)) { 
		std::cerr << "Unrecoverable error in RenderingContext setup." << std::endl;
		return -1;
	}

	std::unique_ptr<Node> root = std::make_unique<Node>();
	root->name = "Root";

	Camera2D* camera = root->add_child(std::make_unique<Camera2D>());
	camera->set_viewport_size(width, height);

	Node2D* node = root->add_child(std::make_unique<Node2D>());
	node->mesh = Mesh::quad(400.0f);

	Uint32 last = SDL_GetTicks();
	int dx, dy;

	float cam_speed = 250.0f;

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

		Vec2 dir = Vec2::zero;

		if (keys[SDL_SCANCODE_W]) {
			dir += Vec2::up;
		}
		if (keys[SDL_SCANCODE_A]) {
			dir += Vec2::left;
		}
		if (keys[SDL_SCANCODE_S]) {
			dir += Vec2::down;
		}
		if (keys[SDL_SCANCODE_D]) {
			dir += Vec2::right;
		}

		dir = dir.normalized() * dt * cam_speed;

		camera->set_position(camera->get_position() + dir);

		// RENDERING //
		glClearColor(0.3f, 0.3f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Vec2 pos        = node->local.position;
		Mat4 model      = Mat4::translate(Vec3(pos.x, pos.y, 0.0f));
		Mat4 view       = camera->get_view();
		Mat4 projection = camera->get_ortho();

		Mat4 mvp = projection * view * model;
		//std::cout << view.to_string() << std::endl;

		node->mesh.render(ctx, mvp);

		ctx.swap_buffers();
	}
	
	ctx.destroy();
	return 0;
}
