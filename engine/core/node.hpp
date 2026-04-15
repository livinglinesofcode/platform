#pragma once

#include <vector>
#include <memory>
#include <math/transform2d.hpp>
#include <rendering/mesh.hpp>

class Node {
protected:
	Node* parent = nullptr;
	std::vector<std::unique_ptr<Node>> children = {};
public:
	virtual ~Node() = default; // polymorphic
	std::string name = "Node";

	Node* get_parent() const { return parent; }

	template <typename T>
	T* add_child(std::unique_ptr<T> child) {
		child->parent = this;

		T* raw = child.get();

		children.push_back(std::move(child));

		return raw;
	}

	const std::vector<std::unique_ptr<Node>>& get_children() const { return children; }
};

class Node2D : public Node {
private:
	mutable Transform2D world;
	mutable bool dirty = true;
public:
	Node2D() { name = "Node2D"; }

	Transform2D local;
	Mesh* mesh = nullptr;

	Transform2D get_world_transform() const {
		Node2D* parent = dynamic_cast<Node2D*>(get_parent());

		if (!parent) {
			return local;
		}

		if (dirty) {
			Transform2D pt = parent->get_world_transform();

			world.scale = pt.scale * local.scale;
			world.set_orientation(pt.get_orientation() * local.get_orientation());
			world.position = pt.position + (local.position * pt.scale).rotate(pt.get_orientation());

			dirty = false;
		}

		return world;
	}

	bool get_dirty() const { return dirty; }
};
