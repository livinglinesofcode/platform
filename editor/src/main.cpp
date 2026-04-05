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
	root->add_child(std::move(editor_camera));

	auto static_body = std::make_unique<StaticBody>();
	static_body->transform.set_position(Vec3::forward * 5.0f);
	root->add_child(std::move(static_body));

	auto renderable = std::make_unique<RenderableNode>();
	renderable->mesh = Mesh::cube_mesh();
	static_body->add_child(std::move(renderable));

	bool running = true;
	SDL_Event e;
	while (running) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) running = false;
		}
			
		glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ctx.swap_buffers();

		for (auto& d : descendants(root_ptr)) {
			if (auto r = dynamic_cast<RenderableNode*>(d)) {
				ctx.render(*r);
			}
		}
	}
	
	ctx.destroy();
	return 0;
}
