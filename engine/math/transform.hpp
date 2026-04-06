#pragma once
#include "vec3.hpp"
#include "quat.hpp"

struct Transform
{
private:
	Vec3 position    = Vec3::zero;
	Quat orientation = Quat::identity;
	Vec3 scale       = Vec3::one;

	bool dirty = true;
public:
	Vec3 get_position() const { return position; }
	void set_position(const Vec3& p) {
		position = p;
		dirty = true;
	}

	Quat get_orientation() const { return orientation; }
	void set_orientation(const Quat& o) {
		orientation = o;
		dirty = true;
	}

	Vec3 get_scale() const { return scale; }
	void set_scale(const Vec3& s) {
		scale = s;
		dirty = true;
	}
};
