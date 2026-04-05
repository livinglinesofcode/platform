attribute vec3 a_position;
uniform mat4 u_mvp; // model-view-projection matrix

void main() {
	gl_Position = u_mvp * vec4(a_position, 1.0);
}
