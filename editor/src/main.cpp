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
	if (!ctx.create_window("Editor", 800, 500)) return -1;

	auto root = std::make_unique<SpatialNode>();
	root->name = "Root";
	auto root_ptr = root.get();

	auto editor_camera = std::make_unique<Camera>();
	editor_camera->transform.set_position(Vec3::back * 3.0f);
	Camera* camera = editor_camera.get();
	root->add_child(std::move(editor_camera));

	auto static_body = std::make_unique<StaticBody>();
	//static_body->transform.set_position(Vec3::forward * 5.0f);
	camera->look_at(static_body.get());

	StaticBody* static_body_ptr = static_body.get();
	root->add_child(std::move(static_body));

	auto renderable = std::make_unique<RenderableNode>();
	renderable->mesh = Mesh::cube(0.5f);
	renderable->transform.set_position(static_body_ptr->get_world_position());
	static_body_ptr->add_child(std::move(renderable));

	bool running = true;
	SDL_Event e;
	while (running) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) running = false;
		}
		
		glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (auto& d : descendants(root_ptr)) {
			if (auto r = dynamic_cast<RenderableNode*>(d)) {
				ctx.render(*r, *camera);
			}
		}

		ctx.swap_buffers();
	}
	
	ctx.destroy();
	return 0;
}
