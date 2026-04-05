#pragma once
#include "vec3.hpp"
#include "quat.hpp"

class Node;

struct Transform
{
private:
	Vec3 position = Vec3::zero;
	Quat rotation = Quat::identity;
	Vec3 scale    = Vec3::one;
	Node* parent;
	bool dirty = true;
public:
	Vec3 get_position() const {
		return position;
	}

	void set_position(const Vec3& p) {
		position = parent ? p - parent->world_position : p;
		dirty = true;
	}

	Quat get_rotation() const {
		return rotation;
	}

	void set_rotation(const Quat& r) {
		rotation = parent ? parent->get_rotation().inverse() * r : r;
		dirty = true;
	}

	Vec3 get_scale() const {
		return scale;
	}

	void set_scale(const Vec3& s) {
		// idk if this is right lmfao i'll fix later
		scale = parent ? parent->get_scale() / s : s;
		dirty = true;
	}
};
