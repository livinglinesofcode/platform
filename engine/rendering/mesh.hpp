#pragma once

#include <glad/glad.h>
#include <math/mat4.hpp>
#include <rendering/abstraction.hpp>

struct Vertex2D {
	float x, y;
};

struct Mesh {
	GLuint  vbo = 0;
	GLuint  ebo = 0;
	GLsizei count = 0;

	Mesh(const Mesh&) = delete;
	Mesh& operator=(const Mesh&) = delete;

	Mesh(Mesh&& other) noexcept {
		vbo = other.vbo;
		ebo = other.ebo;
		count = other.count;

		other.vbo = 0;
		other.ebo = 0;
		other.count = 0;
	}

	Mesh& operator=(Mesh&& other) noexcept {
		if (this != &other) {
			vbo = other.vbo;
			ebo = other.ebo;
			count = other.count;

			other.vbo = 0;
			other.ebo = 0;
			other.count = 0;
		}

		return *this;
	}

	Mesh() = default;
	~Mesh() {
		if (vbo) glDeleteBuffers(1, &vbo);
		if (ebo) glDeleteBuffers(1, &ebo);
	}

	static Mesh& quad() {
		static Mesh m; // no duplicates

		glGenBuffers(1, &m.vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m.vbo);

		glGenBuffers(1, &m.ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.ebo);

		Vertex2D verts[4] = {
			{-0.5,  0.5}, // top left
			{ 0.5,  0.5}, // top right
			{ 0.5, -0.5}, // bottom right
			{-0.5, -0.5}  // bottom left
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

		uint16_t indices[6] = {
			0, 1, 2,
			0, 3, 2
		};

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		m.count = 6;

		return m;
	}

	void render(const RenderingContext& ctx, const Mat4& mvp) {
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glEnableVertexAttribArray(ctx.a_pos);
		glVertexAttribPointer(ctx.a_pos, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*)0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glUniformMatrix4fv(ctx.u_mvp, 1, GL_FALSE, mvp.m);
		glDrawElements(GL_TRIANGLES, this->count, GL_UNSIGNED_SHORT, 0);
	}
};
