#include "vec2.hpp"

const Vec2 Vec2::zero  = Vec2();
const Vec2 Vec2::one   = Vec2(1.0, 1.0);
const Vec2 Vec2::right = Vec2(1.0, 0.0);
const Vec2 Vec2::left  = -Vec2::right;
const Vec2 Vec2::up    = Vec2(0.0, 1.0);
const Vec2 Vec2::down  = -Vec2::up;
