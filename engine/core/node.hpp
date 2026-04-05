#pragma once

#include <vector>
#include <math/transform.hpp>
#include <math/vec3.hpp>
#include <math/quat.hpp>

class Node {
private:
	Node* parent;
	std::vector<Node*> children;
	Transform transform;
public:
	const char* name; // ASCII only
	
	Node* get_parent() const { return parent; }

	void set_parent(Node* p) {
		parent = p;
		p->children.push_back(this);
	}

	const std::vector<Node*>& get_children() const { return children; }

	Vec3 get_world_position() const {
		if (!parent) return transform.get_position();

		return parent->get_world_position() +
			   parent->get_world_rotation() * transform.get_position();
	}

	Quat get_world_rotation() const {
		if (!parent) return transform.get_rotation();

		return parent->get_world_rotation() * transform.get_rotation();
	}
};
