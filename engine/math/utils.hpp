#pragma once

#include <numbers>

constexpr float PI = std::numbers::pi_v<float>;

float radians(float degrees);

bool approx_equal(float a, float b, float eps = 1e-5f);
