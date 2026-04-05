#pragma once

struct Vec2 {
	float x, y;
	
	Vec2() : x(0), y(0) {}
	Vec2(float nx, float ny) : x(nx), y(ny) {}

	Vec2 operator-() const { return {-x, -y}; }

	static const Vec2 zero;
	static const Vec2 one;
	static const Vec2 right;
	static const Vec2 left;
	static const Vec2 up;
	static const Vec2 down;
};
