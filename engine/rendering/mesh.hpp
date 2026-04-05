#pragma once

#include <vector>
#include <math/vec3.hpp>
#include <cstdint>

struct Mesh {
	std::vector<Vec3> vertices;
	std::vector<uint32_t> indices;

	static Mesh* cube_mesh(float size = 1.0f) {
		Mesh* mesh = new Mesh();
		float s = size * 0.5f;

		mesh->vertices = {
			{-s, -s, -s}, { s, -s, -s}, { s,  s, -s}, {-s,  s, -s}, // back face
			{-s, -s,  s}, { s, -s,  s}, { s,  s,  s}, {-s,  s,  s}  // front face
		};

		// 12 tris (2 per face)
		mesh->indices = {
			0,1,2, 2,3,0, // back
			4,5,6, 6,7,4, // front
			0,4,7, 7,3,0, // left
			1,5,6, 6,2,1, // right
			3,2,6, 6,7,3, // top
			0,1,5, 5,4,0  // bottom
		};

		return mesh;
	}
};


