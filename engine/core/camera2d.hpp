#pragma once

#include <core/node.hpp>
#include <math/vec2.hpp>
#include <math/mat4.hpp>
#include <math/quat.hpp>
#include <math/vec3.hpp>
#include <math/utils.hpp>

class Camera2D : public Node2D {
private:
	float zoom = 1.0f;

	int width;
	int height;

	Mat4 ortho;
public:
	void set_viewport_size(int w, int h) {
		width = w; height = h;

		float wh = static_cast<float>(w) / 2.0f;
		float hh = static_cast<float>(h) / 2.0f;
		ortho = Mat4::ortho(-wh, wh, -hh, hh, -1.0f, 1.0f);
		//std::cout << ortho.to_string() << std::endl;
	}

	int get_viewport_width() const { return width; }
	int get_viewport_height() const { return height; }

	Mat4 get_ortho() const { return ortho; }

	Mat4 get_view() const {
		float z = 1.0f / zoom;
		Transform2D t = get_world_transform();
		t.scale = Vec2::one;

		return
			Mat4::scale(Vec3(z, z, 1.0f)) *
			t.inverse();
	}
};
