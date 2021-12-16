#version 430

layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec2 in_UVCoordinates;
out vec2 frag_UVCoordinates;

uniform mat4 u_ViewProjection;
uniform mat4 u_ModelTransform;

void main() {
	gl_Position = u_ViewProjection * u_ModelTransform * vec4(in_Position, 1.0);
	frag_UVCoordinates = in_UVCoordinates;
}
