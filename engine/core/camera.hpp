#pragma once

#include "node.hpp"
#include <math/utils.hpp>

class Camera : public SpatialNode {
private:
	std::pair<uint32_t, uint32_t> viewport_size;
public:
	float fov = radians(70.0f);
	float near = 0.1f;
	float far = 500.0f;

	std::pair<uint32_t, uint32_t> get_viewport_size() const { return viewport_size; }
	void set_viewport_size(uint32_t x, uint32_t y) {
		viewport_size.first = x;
		viewport_size.second = y;
	}

	float aspect() const { return static_cast<float>(viewport_size.first) / static_cast<float>(viewport_size.second); }
};
