#pragma once

#include <glad/glad.h>
#include <math/mat4.hpp>
#include <rendering/abstraction.hpp>

struct Vertex2D {
	float x, y;
};

struct Mesh {
	GLuint  vbo;
	GLuint  ebo;
	GLsizei count;

	Mesh() {
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	}

	static Mesh quad(float s = 1.0f) {
		Mesh m;

		s *= 0.5f;

		Vertex2D verts[4] = {
			{-s,  s}, // top left
			{ s,  s}, // top right
			{ s, -s}, // bottom right
			{-s, -s}  // bottom left
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
