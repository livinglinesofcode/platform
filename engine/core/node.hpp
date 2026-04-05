#pragma once

#include <vector>

class Node {
private:
	Node* parent;
	std::vector<Node*> children;
public:
	const char* name; // ASCII only
	
	Node* get_parent() const { return parent; }

	void set_parent(Node* p) {
		parent = p;
		p->children.push_back(this);
	}

	const std::vector<Node*>& get_children() const { return children; }
};
