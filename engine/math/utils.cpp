#include <math/utils.hpp>
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>

float radians(float degrees) {
	return degrees * (PI / 180.0f);
}

bool approx_equal(float a, float b, float eps) {
	return std::fabs(a - b) <= eps;
}

std::string format_float(float value) {
	std::ostringstream ss;
	ss << std::fixed << std::setprecision(2) << value;
	std::string s = ss.str();

	s.erase(s.find_last_not_of('0') + 1, std::string::npos);
	if (!s.empty() && s.back() == '.') s.pop_back();

	return s;
}
