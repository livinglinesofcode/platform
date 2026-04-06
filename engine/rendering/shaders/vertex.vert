attribute vec3 a_pos;
uniform mat4 u_mvp; // model-view-projection matrix

void main() {
	gl_Position = u_mvp * vec4(a_pos, 1.0);
}
