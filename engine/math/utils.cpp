#include <math/utils.hpp>
#include <cmath>

float radians(float degrees) {
	return degrees * (PI / 180.0f);
}

bool approx_equal(float a, float b, float eps) {
	return std::fabs(a - b) <= eps;
}
