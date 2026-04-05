#include "node.hpp"

const std::vector<Node*> descendants(Node* node) {
	std::vector<Node*> out;

	for (auto& child : node->get_children()) {
		auto ptr = child.get();
		out.push_back(ptr);
		auto child_desc = descendants(ptr);
		out.insert(out.end(), child_desc.begin(), child_desc.end());
	}

	return out;
}
