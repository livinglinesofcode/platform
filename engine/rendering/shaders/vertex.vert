attribute vec2 a_pos;
uniform mat4 u_ortho;

void main() {
	gl_Position = u_ortho * vec4(a_pos, 0.0, 1.0);
}
