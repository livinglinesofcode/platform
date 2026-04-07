#include "vec3.hpp"

const Vec3 Vec3::zero    = Vec3();
const Vec3 Vec3::one     = Vec3();
const Vec3 Vec3::right   = Vec3(1.0f,0.0f,0.0f);
const Vec3 Vec3::left    = -Vec3::right;
const Vec3 Vec3::up      = Vec3(0.0f,1.0f,0.0f);
const Vec3 Vec3::down    = -Vec3::up;
const Vec3 Vec3::forward = Vec3(0.0f,0.0f,1.0f);
const Vec3 Vec3::back    = -Vec3::forward;
