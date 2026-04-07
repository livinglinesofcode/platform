#include <SDL_keyboard.h>
#include <SDL_mouse.h>
#include <SDL_timer.h>
#include <memory>
#include <rendering/abstraction.hpp>
#include <core/node.hpp>
#include <core/node.cpp>
#include <core/camera.hpp>
#include <math/vec3.hpp>
#include <physics/static_body.hpp>
#include <rendering/mesh.hpp>
#include <sys/stat.h>

int main() {
	RenderingContext ctx;

	const uint16_t width = 800;
	const uint16_t height = 500;
	if (!ctx.create_window("Editor", width, height)) return -1;

	auto root = std::make_unique<SpatialNode>();
	root->name = "Root";
	auto root_ptr = root.get();

	auto editor_camera = std::make_unique<Camera>();
	Camera* camera = editor_camera.get();
	camera->set_viewport_size(width, height);
	root->add_child(std::move(editor_camera));

	auto static_body = std::make_unique<StaticBody>();
	//static_body->transform.set_position(Vec3::forward * 5.0f);

	StaticBody* static_body_ptr = static_body.get();
	root->add_child(std::move(static_body));

	auto renderable = std::make_unique<RenderableNode>();
	renderable->mesh = Mesh::cube(1.0f);
	renderable->transform.set_position(Vec3::back * 5.0f);
	camera->look_at(renderable.get());
	renderable->name = "Renderable";

	//std::cout << "Renderable world position: " << renderable->get_world_position().to_string() << std::endl;
	//std::cout << "Renderable local position: " << renderable->transform.get_position().to_string() << std::endl;
	//std::cout << "Camera world position: " << camera->get_world_position().to_string() << std::endl;
	//std::cout << "Camera forward: " << camera->get_forward().to_string() << std::endl;

	static_body_ptr->add_child(std::move(renderable));

	int dx, dy;
	const float cam_speed = 5.0f;
	const float sensitivity = 1.5f;
	const float rad = 89.9f * (3.14159265f / 180.0f);
	float pitch = 0.0f;
	float yaw = 0.0f;

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

		if (buttons & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
			Vec3 dir = Vec3::zero;

			if (keys[SDL_SCANCODE_W]) {
				dir += camera->get_forward();
			}
			if (keys[SDL_SCANCODE_A]) {
				dir -= camera->get_right();
			}
			if (keys[SDL_SCANCODE_S]) {
				dir -= camera->get_forward();
			}
			if (keys[SDL_SCANCODE_D]) {
				dir += camera->get_right();
			}

			if (dir.length() > 0.0f) {
				dir = dir.normalized();
			}

			camera->transform.set_position(camera->transform.get_position() + dir * dt * cam_speed);

			// camera orientation input
			if (buttons) {
				yaw += sensitivity * dx * dt;
				pitch += sensitivity * dy * dt;
				pitch = std::clamp(pitch, -rad, rad);

				Quat q_yaw = Quat(Vec3::up, yaw);
				Quat q_pitch = Quat(Vec3::right, pitch);

				camera->transform.set_orientation(q_yaw * q_pitch);
			}

			std::cout << camera->transform.get_position().to_string() << " " << camera->transform.get_orientation().to_string() << std::endl;
		}

		glClearColor(0.3f, 0.3f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (auto& d : descendants(root_ptr)) {
			if (auto r = dynamic_cast<RenderableNode*>(d)) {
				//std::cout << r->name << std::endl;
				ctx.render(*r, *camera);
			}
		}

		ctx.swap_buffers();
	}
	
	ctx.destroy();
	return 0;
}
