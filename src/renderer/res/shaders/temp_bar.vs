#version 440


layout(location = 0) in vec2 in_Position;
layout(location = 1) in vec2 in_UVCoordinates;

out vec2 frag_UVCoordinates;

uniform mat4 u_View;

void main() {
    gl_Position = u_View * vec4(in_Position, 0.0, 1.0);
    // gl_Position = vec4(in_UVCoordinates - 0.5, 0.0, 1.0);
    // vec4 pos = u_View * vec4(in_Position, 0.0, 1.0);
	frag_UVCoordinates = in_UVCoordinates; // vec2(pos[0], pos[1]);
}