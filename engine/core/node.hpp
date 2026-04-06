#pragma once

#include <vector>
#include <memory>
#include <math/transform.hpp>
#include <math/vec3.hpp>
#include <math/quat.hpp>
#include <rendering/mesh.hpp>
#include <math/utils.cpp>
#include <algorithm>

class Node {
protected:
	Node* parent = nullptr;
	std::vector<std::unique_ptr<Node>> children = {};
public:
	virtual ~Node() = default; // polymorphic
	const char* name = "Node"; // ASCII only
	
	Node* get_parent() const { return parent; }

	virtual void add_child(std::unique_ptr<Node> child) {
		child->parent = this;
		children.push_back(std::move(child));
	}

	const std::vector<std::unique_ptr<Node>>& get_children() const { return children; }
};

class SpatialNode : public Node {
public:
	Transform transform;
	// SpatialNodes can only be children of a SpatialNode parent
	const SpatialNode* spatial_parent = nullptr;

	void add_child(std::unique_ptr<Node> child) {
		if (auto spatial = dynamic_cast<SpatialNode*>(child.get())) {
			spatial->spatial_parent = this;
		}

		Node::add_child(std::move(child));
	}

	Vec3 get_world_position() const {
		if (!spatial_parent) return transform.get_position();

		return spatial_parent->get_world_position() +
			   spatial_parent->get_world_orientation() * transform.get_position();
	}

	Quat get_world_orientation() const {
		if (!get_parent()) return transform.get_orientation();

		return spatial_parent->get_world_orientation() * transform.get_orientation();
	}

	Vec3 get_right() const { return get_world_orientation() * Vec3::right; }
	Vec3 get_up() const { return get_world_orientation() * Vec3::up; }
	Vec3 get_forward() const { return get_world_orientation() * Vec3::forward; }

	void look_at(SpatialNode* target) {
		Vec3 dir = (target->get_world_position() - this->get_world_position()).normalized();

		Vec3 axis = Vec3::forward.cross(dir);
		float len = axis.length();
		float dot = Vec3::forward.dot(dir);

		// parallel or opposite vectors
		if (approx_equal(len, 0.0f)) {
			if (dot > 0.0f) {
				// already facing target
				return;
			}
			else {
				// facing opposite directions
				transform.set_orientation(transform.get_orientation() * Quat(Vec3::up, radians(180)));
				return;
			}
		}

		axis = axis.normalized();
		float angle = static_cast<float>(std::acos(std::clamp(dot, -1.0f, 1.0f)));

		this->transform.set_orientation(Quat(axis, angle));
	}
};

class RenderableNode : public SpatialNode {
public:
	Mesh* mesh;
	bool visible = true;
};
