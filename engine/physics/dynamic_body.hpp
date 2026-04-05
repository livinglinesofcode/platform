#pragma once

#include <core/node.hpp>
#include <math/vec3.hpp>

class DynamicBody : public SpatialNode {
public:
	Vec3 linear_velocity;
	Vec3 angular_velocity;
};
