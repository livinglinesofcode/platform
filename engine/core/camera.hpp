#pragma once

#include "node.hpp"

class Camera : public SpatialNode {
private:
	float near = 0.1f;
	float far = 1000.0f;
public:
	float fov = 70.0f;
};
