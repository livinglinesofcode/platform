#pragma once

#include <vector>
#include <memory>
#include <math/transform.hpp>

class Node {
protected:
	Node* parent = nullptr;
	std::vector<std::unique_ptr<Node>> children = {};
public:
	virtual ~Node() = default; // polymorphic
	std::string name = "Node";

	Node* get_parent() const { return parent; }

	Node* add_child(std::unique_ptr<Node> child) {
		child->parent = this;

		Node* raw = child.get();

		children.push_back(std::move(child));

		return raw;
	}

	const std::vector<std::unique_ptr<Node>>& get_children() const { return children; }
};

class SpatialNode : public Node {
public:
	SpatialNode() { name = "SpatialNode"; }

	Transform local;
};
