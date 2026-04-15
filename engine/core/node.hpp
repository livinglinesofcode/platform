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

	virtual void mark_dirty() {
		for (const auto& child : children) {
			child.get()->mark_dirty();
		}
	}

	const std::vector<Node*> get_children() const {
		std::vector<Node*> vec;

		for (const std::unique_ptr<Node>& c : children) {
			vec.push_back(c.get());
		}

		return vec;
	}
};

class Node2D : public Node {
private:
	mutable Transform2D local;
	mutable Transform2D world;
	mutable bool dirty = true;
public:
	Node2D() { name = "Node2D"; }

	Mesh* mesh = nullptr;
	int z_index;

	Transform2D get_world_transform() const {
		Node2D* parent = dynamic_cast<Node2D*>(get_parent());

		if (!parent) {
			return local;
		}

		if (dirty || parent->dirty) {
			Transform2D pt = parent->get_world_transform();

			world.scale = pt.scale * local.scale;
			world.set_orientation(pt.get_orientation() * local.get_orientation());
			world.position = pt.position + local.position.rotate(pt.get_orientation());

			dirty = false;
		}

		return world;
	}

	void mark_dirty() override {
		dirty = true;
		Node::mark_dirty();
	}

	Vec2 get_position() const { return local.position; }
	void set_position(const Vec2& v) {
		local.position = v;
		mark_dirty();
	}

	float get_orientation() const { return local.get_orientation(); }
	void set_orientation(float o) {
		local.set_orientation(o);
		mark_dirty();
	}

	Vec2 get_scale() const { return local.scale; }
	void set_scale(const Vec2& s) {
		local.scale = s;
		mark_dirty();
	}
};
