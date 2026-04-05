#pragma once

#include <cmath>

struct Vec3 {
	float x, y, z;
	
	Vec3() : x(0), y(0), z(0) {}
	Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
	
	Vec3 operator+(const Vec3& v) const { return {x+v.x, y+v.y, z+v.z}; }
	Vec3 operator-() const { return {-x, -y, -z}; }
	Vec3 operator-(const Vec3& v) const { return *this + (-v); }
	Vec3 operator*(float s) const { return {x*s, y*s, z*s}; }
	Vec3 operator/(float s) const {
		if (s != 0.0f) {
			float reciprocal = 1.0f / s;
			return Vec3{x * reciprocal, y * reciprocal, z * reciprocal};
		}

		return -Vec3::one;
	}
	
	Vec3& operator+=(const Vec3& v) { x+=v.x; y+=v.y; z+=v.z; return *this; }
	Vec3& operator-=(const Vec3& v) { x-=v.x; y-=v.y; z-=v.z; return *this; }
	Vec3& operator*=(float s) { x*=s; y*=s; z*=s; return *this; }
	Vec3& operator/=(float s) { if (s != 0) { x/=s; y/=s; z/=s; } return *this; }
	
	bool operator==(const Vec3& v) const {
		float eps = 1e-6f;
		return std::abs(x - v.x) < eps &&
			   std::abs(y - v.y) < eps &&
			   std::abs(z - v.z) < eps;
	}
	bool operator!=(const Vec3& v) const {
		return !(*this == v);
	}
	
	float length() const { return std::sqrt(x*x + y*y + z*z); }
	Vec3 normalized() const { 
		float len = length();
		return len != 0 ? Vec3{x/len, y/len, z/len} : Vec3::zero;
	}

	Vec3 cross(const Vec3& b) const {
		return Vec3{
			y * b.z - z * b.y,
			z * b.x - x * b.z,
			x * b.y - y * b.x
		};
	}

	float dot(const Vec3& b) const {
		return x*b.x + y*b.y + z*b.z;
	}

	static const Vec3 zero;
	static const Vec3 one;
	static const Vec3 right;
	static const Vec3 left;
	static const Vec3 up;
	static const Vec3 down;
	static const Vec3 forward;
	static const Vec3 back;
};
