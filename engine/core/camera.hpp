#pragma once
#include "math/transform.hpp"

class Camera {
	public:
		Transform transform;
		float fov = 70.0f;
	private:
		float near = 0.1f;
		float far = 1000.0f;
};
