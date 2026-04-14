#pragma once

#include "vec2.hpp"

struct Transform2D {
	Vec2  position    = Vec2::zero;
	float orientation = 0;
	Vec2  scale       = Vec2::one;
};
