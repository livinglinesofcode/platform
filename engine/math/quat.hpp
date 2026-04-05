#pragma once

#include <cmath>
#include "vec3.hpp"

struct Quat {
	float x, y, z, w;

	Quat() : x(0), y(0), z(0), w(1) {}

	Quat(float pitch, float yaw, float roll) {
		float cp = static_cast<float>(cos(pitch * 0.5f));
		float sp = static_cast<float>(sin(pitch * 0.5f));
		float cy = static_cast<float>(cos(yaw * 0.5f));
		float sy = static_cast<float>(sin(yaw * 0.5f));
		float cr = static_cast<float>(cos(roll * 0.5f));
		float sr = static_cast<float>(sin(roll * 0.5f));

		x = sr * cp * cy - cr * sp * sy;
		y = cr * sp * cy + sr * cp * sy;
		z = cr * cp * sy - sr * sp * cy;
		w = cr * cp * cy + sr * sp * sy;
	}

	Quat(float nx, float ny, float nz, float nw) : x(nx), y(ny), z(nz), w(nw) {}

	Quat(Vec3 axis, float radians) {
		axis = axis.normalized();
		float s = static_cast<float>(sin(radians / 2.0f));
		w = static_cast<float>(cos(radians / 2.0f));
		x = axis.x * s;
		y = axis.y * s;
		z = axis.z * s;
	}

	static const Quat identity;

	Quat inverse() const {
		return {-x, -y, -z, w};
	}

	void normalize() {
		float len = std::sqrt(x*x + y*y + z*z + w*w);
		if (len > 0.0f) {
			float reciprocal = 1.0f / len;
			x *= reciprocal;
			y *= reciprocal;
			z *= reciprocal;
			w *= reciprocal;
		}
	}

	Quat operator*(const Quat& b) const {
		return {
			w*b.x + x*b.w + y*b.z - z*b.y,
			w*b.y - x*b.z + y*b.w + z*b.x,
			w*b.z + x*b.y - y*b.x + z*b.w,
			w*b.w - x*b.x - y*b.y - z*b.z
		};
	}

	Vec3 operator*(const Vec3& v) const {
		Vec3 qv{x, y, z};

		Vec3 t = qv.cross(v) * 2.0f;
		return v + t * w + qv.cross(t);
	}
};
