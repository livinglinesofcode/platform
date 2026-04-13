#include <SDL_keyboard.h>
#include <SDL_mouse.h>
#include <SDL_timer.h>
#include <memory>
#include <rendering/abstraction.hpp>
#include <core/node.hpp>
#include <core/camera.hpp>
#include <math/vec3.hpp>
#include <physics/static_body.hpp>
#include <rendering/mesh.hpp>
#include <sys/stat.h>
#include <math/utils.hpp>

int main() {
	RenderingContext ctx;

	const uint16_t width = 800;
	const uint16_t height = 500;
	if (!ctx.create_window("Editor", width, height)) return -1;

	std::unique_ptr<Node> root = std::make_unique<Node>();
	root->name = "Root";

	Camera* camera = static_cast<Camera*>(
		root->add_child(std::make_unique<Camera>())
	);
	camera->local.position = Vec3::up * 5.0f;
	camera->set_viewport_size(width, height);

	ctx.upload_grid(50.0f, 5.0f);

	int dx, dy;
	const float cam_speed = 25.0f;
	const float sensitivity = 0.5f;
	const float rad = radians(89.9f);
	float yaw = 0.0f;   // horizontal turn (-180, 180)
	float pitch = 0.0f; // vertical tilt   (-90, 90)

	bool running = true;
	SDL_Event e;
	Uint32 last = SDL_GetTicks();
	while (running) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) running = false;
		}

		Uint32 now = SDL_GetTicks();
		float dt = (now - last) / 1000.0f; // seconds
		last = now;

		const Uint8* keys = SDL_GetKeyboardState(nullptr);
		Uint32 buttons = SDL_GetRelativeMouseState(&dx, &dy);

		// FIXME: buggy W and S, pitch zooms in
		if (buttons & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
			// camera orientation input
			if (buttons) {
				yaw += sensitivity * dx * dt;
				if (yaw >  PI) yaw -= TAU;
				if (yaw < -PI) yaw += TAU;

				pitch -= sensitivity * dy * dt;
				pitch = std::clamp(pitch, -rad, rad);

				Quat q_yaw = Quat(Vec3::up, yaw);
				Quat q_pitch = Quat(Vec3::right, pitch);

				camera->local.orientation = q_yaw * q_pitch;
			}

			printf("(%.0f, %.0f)\n", degrees(yaw), degrees(pitch));

			// camera position input
			Vec3 dir = Vec3::zero;

			if (keys[SDL_SCANCODE_W]) {
				dir += Vec3::forward;
			}
			if (keys[SDL_SCANCODE_A]) {
				dir += Vec3::left;
			}
			if (keys[SDL_SCANCODE_S]) {
				dir += Vec3::back;
			}
			if (keys[SDL_SCANCODE_D]) {
				dir += Vec3::right;
			}

			if (dir.length() > 0.0f) {
				dir = dir.normalized();
			}

			camera->local.position += dir * dt * cam_speed;
		}

		glClearColor(0.3f, 0.3f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ctx.render_grid(*camera);

		ctx.swap_buffers();
	}
	
	ctx.destroy();
	return 0;
}
