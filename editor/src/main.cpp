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

struct Renderable {
	Node2D* node;
	Transform2D world;
};

void gather_renderables(Node* node, std::vector<Renderable>& out) {
	if (auto* n2d = dynamic_cast<Node2D*>(node)) {
		if (n2d->mesh) {
			out.push_back({
				n2d,
				n2d->get_world_transform()
			});
		}
	}

	for (const auto& child : node->get_children()) {
		gather_renderables(child, out);
	}
}

void render_root(const RenderingContext& ctx, Node* root, const Mat4& view, const Mat4& ortho) {
	std::vector<Renderable> r;
	gather_renderables(root, r);
	std::sort(r.begin(), r.end(), [](const Renderable& a, const Renderable& b) {
		return a.node->z_index < b.node->z_index;
	});

	Mat4 vp = ortho * view;

	for (auto renderable : r) {
		Mat4 mvp = vp * renderable.world.model();
		renderable.node->mesh->render(ctx, mvp);
	}
}

int main() {
	RenderingContext ctx;

	const int width  = 800;
	const int height = 500;
	if (!ctx.create_window("Editor", width, height)) { 
		std::cerr << "Unrecoverable error in RenderingContext setup." << std::endl;
		return -1;
	}

	std::unique_ptr<Node> root = std::make_unique<Node>();
	Node* root_ptr = root.get();
	root_ptr->name = "Root";

	Node2D* cube = root->add_child(std::make_unique<Node2D>());
	cube->mesh = &Mesh::quad();
	cube->set_position(Vec2::right * 200.0f);
	cube->set_scale(Vec2::one * 150.0f);
	cube->set_orientation(PI/4.0f);

	Node2D* cube2 = cube->add_child(std::make_unique<Node2D>());
	cube2->mesh = &Mesh::quad();
	cube2->set_position(Vec2::right * 50.0f);
	cube2->set_scale(Vec2::one);

	Camera2D* camera = root->add_child(std::make_unique<Camera2D>());
	camera->set_viewport_size(width, height);

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

		cube->set_position(cube->get_position() + dir.normalized() * dt * cam_speed);

		// RENDERING //
		glClearColor(0.3f, 0.3f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		render_root(ctx, root_ptr, camera->get_view(), camera->get_ortho());
		ctx.swap_buffers();
	}
	
	ctx.destroy();
	return 0;
}
