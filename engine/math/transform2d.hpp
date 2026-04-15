#pragma once

#include "vec2.hpp"
#include <math/utils.hpp>
#include <math/mat4.hpp>

struct Transform2D {
private:
	float orientation = 0.0f; // radians
public:
	Vec2  position    = Vec2::zero;
	Vec2  scale       = Vec2::one;

	float get_orientation() const { return orientation; }

	void set_orientation(float o) {
		orientation = std::fmod(o, TAU);
	}

	Mat4 inverse() const {
		float sx = (scale.x != 0.0f) ? 1.0f / scale.x : 0.0f;
		float sy = (scale.y != 0.0f) ? 1.0f / scale.y : 0.0f;

		Vec2 p = position;

		// undo scale
		p.x *= sx;
		p.y *= sy;

		// undo rotation
		float o = -orientation;
		float s = static_cast<float>(std::sin(o));
		float c = static_cast<float>(std::cos(o));

		p = {
			p.x * c - p.y * s,
			p.x * s + p.y * c
		};

		// undo translation
		p = -p;

		return
			Mat4::scale(Vec3(sx, sy, 1.0f)) *
			Mat4::rotate(Quat(Vec3(0.0f, 0.0f, 1.0f), o)) *
			Mat4::translate(Vec3(p.x, p.y, 0.0f));
	}
};
