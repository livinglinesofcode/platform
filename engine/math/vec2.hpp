#pragma once

#include <cmath>
#include <format>
#include <math/utils.hpp>
#include <algorithm>

struct Vec2 {
	float x, y;
	
	Vec2() : x(0), y(0) {}
	Vec2(float nx, float ny) : x(nx), y(ny) {}

	Vec2 operator-() const { return {-x, -y}; }
	Vec2 operator-(const Vec2& v) const { return {x - v.x, y - v.y}; }

	Vec2 operator+(const Vec2& v) const { return { x + v.x, y + v.y }; }
	Vec2 operator+=(const Vec2& v) { x += v.x; y += v.y; return *this; }

	Vec2 operator*(float s) const { return {x*s, y*s}; }
	Vec2 operator*(const Vec2& v) const { return {x * v.x, y * v.y}; }

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

	Vec2 rotate(float r) const {
		float s = static_cast<float>(sin(r));
		float c = static_cast<float>(cos(r));

		return {
			x * c - y * s,
			x * s + y * c
		};
	}

	Vec2 lerp(const Vec2& a, const Vec2& b, float t) const {
		t = std::clamp(t, 0.0f, 1.0f);
		return a + (b - a) * t;
	}

	std::string to_string() const {
		return std::format("Vec2({}, {})", format_float(x), format_float(y));
	}

	static const Vec2 zero;
	static const Vec2 one;
	static const Vec2 right;
	static const Vec2 left;
	static const Vec2 up;
	static const Vec2 down;
};
