#pragma once

#include "node.hpp"
#include <math/utils.hpp>
#include <math/mat4.hpp>

class Camera : public SpatialNode {
private:
	std::pair<uint16_t, uint16_t> viewport_size;
	Mat4 projection;

	float fov = radians(70.0f);
	float near = 0.1f;
	float far = 500.0f;
public:
	Mat4 get_projection() const {
		return projection;
	}

	void update_projection_matrix() {
		projection = Mat4::perspective(fov, aspect(), near, far);
	}

	std::pair<uint16_t, uint16_t> get_viewport_size() const { return viewport_size; }
	void set_viewport_size(uint16_t x, uint16_t y) {
		viewport_size.first = x;
		viewport_size.second = y;
		update_projection_matrix();
	}

	float aspect() const { return static_cast<float>(viewport_size.first) / static_cast<float>(viewport_size.second); }
};
