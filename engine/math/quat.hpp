#pragma once
#include "vec3.hpp"
#include <cmath>

struct Quat {
	float x, y, z, w;

	Quat() : x(0), y(0), z(0), w(1) {}

	Quat(float pitch, float yaw, float roll) {
		float cp = cos(pitch * 0.5f);
		float sp = sin(pitch * 0.5f);
		float cy = cos(yaw * 0.5f);
		float sy = sin(yaw * 0.5f);
		float cr = cos(roll * 0.5f);
		float sr = sin(roll * 0.5f);

		x = sr * cp * cy - cr * sp * sy;
		y = cr * sp * cy + cr * cp * sy;
		z = cr * cp * sy - sr * sp * cy;
		w = cr * cp * cy + sr * sp * cy;
	}

	Quat(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

	Quat(Vec3 axis, float radians) {
		axis = axis.normalized();
		float s = sin(radians / 2.0f);
		w = cos(radians / 2.0f);
		x = axis.x * s;
		y = axis.y * s;
		z = axis.z * s;
	}

	static const Quat identity;

	Quat inverse() const {
		return {-x, -y, -z, w};
	}
};
