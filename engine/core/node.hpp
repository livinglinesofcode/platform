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
public:
	Node2D() { name = "Node2D"; }

	Transform2D local;
	Mesh mesh;
};
