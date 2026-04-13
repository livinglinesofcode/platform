#pragma once

#include "vec3.hpp"
#include "quat.hpp"

struct Transform {
	Vec3 position    = Vec3::zero;
	Quat orientation = Quat::identity;
	Vec3 scale       = Vec3::one;
};
