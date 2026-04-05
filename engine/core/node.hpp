#pragma once

#include <vector>
#include <memory>
#include <math/transform.hpp>
#include <math/vec3.hpp>
#include <math/quat.hpp>
#include <rendering/mesh.hpp>

class Node {
private:
	Node* parent = nullptr;
	std::vector<std::unique_ptr<Node>> children = {};
public:
	virtual ~Node() {} // polymorphic
	const char* name = "Node"; // ASCII only
	
	Node* get_parent() const { return parent; }
	Node* create_child() {
		children.push_back(std::make_unique<Node>());
		Node* child = children.back().get();
		child->parent = this;
		return child;
	}
	void add_child(std::unique_ptr<Node> child) {
		child->parent = this;
		parent->children.push_back(std::move(child));
	}

	const std::vector<std::unique_ptr<Node>>& get_children() const { return children; }
};

class SpatialNode : public Node {
public:
	Transform transform;
	// SpatialNodes can only be children of a SpatialNode parent
	const SpatialNode* spatial_parent = static_cast<const SpatialNode*>(get_parent());

	Vec3 get_world_position() const {
		if (!spatial_parent) return transform.get_position();

		return spatial_parent->get_world_position() +
			   spatial_parent->get_world_rotation() * transform.get_position();
	}

	Quat get_world_rotation() const {
		if (!get_parent()) return transform.get_rotation();

		return spatial_parent->get_world_rotation() * transform.get_rotation();
	}

	Vec3 get_right() const { return get_world_rotation() * Vec3::right; }
	Vec3 get_up() const { return get_world_rotation() * Vec3::up; }
	Vec3 get_forward() const { return get_world_rotation() * Vec3::forward; }
};

class RenderableNode : public Node {
public:
	Mesh* mesh;
	bool visible = true;
};
