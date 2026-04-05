#pragma once
#include "math/vec2.hpp"

struct Rect {
	Vec2 pos;
	Vec2 size;

	bool contains(int px, int py) {
		return px >= pos.x && px <= pos.x + size.x &&
			   py >= pos.y && py <= pos.y + size.y;
	}
};
