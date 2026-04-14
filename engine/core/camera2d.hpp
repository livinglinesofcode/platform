#pragma once

#include "node.hpp"
#include <math/mat4.hpp>

class Camera2D : public Node2D {
private:
	int width;
	int height;
	Mat4 ortho;
public:
	void set_viewport_size(int w, int h) {
		width = w; height = h;

		float wh = static_cast<float>(w) / 2.0f;
		float hh = static_cast<float>(h) / 2.0f;
		ortho = Mat4::ortho(-wh, wh, -hh, hh, -1.0f, 1.0f);
	}

	int get_viewport_width() const { return width; }
	int get_viewport_height() const { return height; }

	Mat4 get_ortho() const { return ortho; }
};
