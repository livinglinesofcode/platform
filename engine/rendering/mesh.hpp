#pragma once

#include <glad/glad.h>
#include <vector>
#include <cstdint>

struct Vertex {
	float x, y, z;
};

struct Mesh {
	std::vector<Vertex> vertices;
	std::vector<uint16_t> indices;

	GLuint VBO = 0;
	GLuint EBO = 0;

	void upload_to_gpu() {
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(Vertex)), vertices.data(), GL_STATIC_DRAW);

		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indices.size() * sizeof(uint16_t)), indices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	static Mesh* cube(float size = 1.0f) {
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

		mesh->upload_to_gpu();
		return mesh;
	}
};
