#pragma once

#include "vec3.hpp"
#include "quat.hpp"

struct Mat4 {
	float m[16]; // collumn-major

	static Mat4 identity() {
		Mat4 mat = {};
		mat.m[0] = mat.m[5] = mat.m[10] = mat.m[15] = 1.0f;
		return mat;
	}

	static Mat4 translate(const Vec3& v) {
		Mat4 mat = identity();
		mat.m[12] = v.x;
		mat.m[13] = v.y;
		mat.m[14] = v.z;
		return mat;
	}

	static Mat4 rotate(const Quat& q) {
		Mat4 mat = identity();
		float xx = q.x * q.x, yy = q.y * q.y, zz = q.z * q.z;
		float xy = q.x*q.y, xz = q.x*q.z, yz = q.y*q.z;
		float wx = q.w*q.x, wy = q.w*q.y, wz = q.w*q.z;

		mat.m[0] = 1 - 2*(yy+zz);
		mat.m[1] = 2*(xy + wz);
		mat.m[2] = 2*(xz - wy);

		mat.m[4] = 2*(xy - wz);
		mat.m[5] = 1 - 2*(xx+zz);
		mat.m[6] = 2*(yz + wx);

		mat.m[8] = 2*(xz + wy);
		mat.m[9] = 2*(yz - wx);
		mat.m[10] = 1 - 2*(xx+yy);

		return mat;
	}

	Mat4 operator*(const Mat4& rhs) const {
		Mat4 result = {};

		for(int row=0; row<4; ++row) {
			for(int col=0; col<4; ++col) {
				result.m[col*4 + row] =
					m[0*4 + row]*rhs.m[col*4 + 0] +
					m[1*4 + row]*rhs.m[col*4 + 1] +
					m[2*4 + row]*rhs.m[col*4 + 2] +
					m[3*4 + row]*rhs.m[col*4 + 3];
			}
		}

		result.m[15] = 1.0f;
		return result;
	}

	Mat4 operator*=(const Mat4& rhs) {
		return *this * rhs;
	}

	static Mat4 perspective(float fov, float aspect, float near, float far) {
		Mat4 mat = {};
		float tan_half_fov = std::tan(fov * 0.5f);

		mat.m[0] = 1.0f / (aspect * tan_half_fov);
		mat.m[5] = 1.0f / (tan_half_fov);
		mat.m[10] = -(far + near) / (far - near);
		mat.m[11] = -1.0f;
		mat.m[14] = -(2*far*near)/(far - near);

		return mat;
	}

	static Mat4 look_at(const Vec3& eye, const Vec3& center, const Vec3& up = Vec3::up) {
		Vec3 f = (center - eye).normalized();
		Vec3 s = f.cross(up).normalized();
		Vec3 u = s.cross(f);

		Mat4 mat = identity();
		mat.m[0] = s.x; mat.m[4] = s.y; mat.m[8] = s.z;
		mat.m[1] = u.x; mat.m[5] = u.y; mat.m[9] = u.z;
		mat.m[2] = -f.x; mat.m[6] = -f.y; mat.m[10] = -f.z;

		mat.m[12] = -s.dot(eye);
		mat.m[13] = -u.dot(eye);
		mat.m[14] = f.dot(eye);
		return mat;
	}
};
