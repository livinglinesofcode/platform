#pragma once

#include <cmath>
struct Vec2 {
	float x, y;
	
	Vec2() : x(0), y(0) {}
	Vec2(float nx, float ny) : x(nx), y(ny) {}

	Vec2 operator-() const { return {-x, -y}; }
	Vec2 operator-(const Vec2& v) { return {x - v.x, y - v.y}; }

	Vec2 operator+(const Vec2& v) { return { x + v.x, y + v.y }; }
	Vec2 operator+=(const Vec2& v) { x += v.x; y += v.y; return *this; }

	Vec2 operator*(float s) const { return {x*s, y*s}; }

	float length() const {
		return std::sqrt(x*x + y*y);
	}

	float length_squared() const {
		return x*x + y*y;
	}

	Vec2 normalized() const {
		float len = length();
		if (len == 0) { return Vec2::zero; }

		return { x/len, y/len };
	}

	static const Vec2 zero;
	static const Vec2 one;
	static const Vec2 right;
	static const Vec2 left;
	static const Vec2 up;
	static const Vec2 down;
};
