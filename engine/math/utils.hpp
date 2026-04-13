#pragma once

#include <numbers>
#include <string>

constexpr float PI  = std::numbers::pi_v<float>;
constexpr float TAU = 2.0f * PI;

float radians(float degrees);
float degrees(float radians);

bool approx_equal(float a, float b, float eps = 1e-5f);

std::string format_float(float value);
