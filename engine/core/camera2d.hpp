#pragma once

#include <core/node.hpp>
#include <math/vec2.hpp>
#include <math/mat4.hpp>
#include <math/quat.hpp>
#include <math/vec3.hpp>
#include <math/utils.hpp>
//#include <iostream>

// TODO: add cached world space to Node2D + dirty flag and render using camera world space
class Camera2D : public Node {
private:
	Vec2 position = Vec2::zero;
	float orientation = 0.0f;
	float zoom = 1.0f;

	int width;
	int height;

	Mat4 ortho;

	mutable bool dirty = true;
	mutable Mat4 view;
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
		if (dirty) {
			float z = 1.0f / zoom;

			view =
				Mat4::rotate(Quat(Vec3(0.0f, 0.0f, 1.0f), -orientation)) *
				Mat4::scale(Vec3(z, z, 1.0f)) *
				Mat4::translate(Vec3(-position.x, -position.y, 0));

			dirty = false;
		}

		return view;
	}

	void set_position(const Vec2& v) { 
		position = v;
		dirty = true;
	}
	void set_orientation(float o) {
		orientation = std::fmod(o, TAU);
		dirty = true;
	}

	Vec2 get_position() const { return position; }
	float get_orientation() const { return orientation; }
};
