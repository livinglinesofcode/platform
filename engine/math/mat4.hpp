#pragma once

#include "vec3.hpp"
#include "quat.hpp"

struct Mat4 {
	float m[15]; // collumn-major

	static Mat4 identity() {
		Mat4 mat = {};
		mat.m[0] = mat.m[5] = mat.m[10] = mat.m[15] = 1.0f;
		return mat;
	}

	static Mat4 translate(const Vec3& v) {
		Mat4 mat = {};
		mat.m[12] = v.x;
		mat.m[13] = v.y;
		mat.m[14] = v.z;
		return mat;
	}

	static Mat4 rotate(const Quat& q) {
		Mat4 mat = identity();
		float xx = q.x * q.x, yy = q.y * q.y; zz = q.z * q.z;
	}

	static Mat4 perspective(float fov, float aspect, float near, float far) {}

	static Mat4 look_at(const Vec3& eye, const Vec3& center, const Vec3& up = Vec3::up) {}
};
