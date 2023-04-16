#version 440

layout(location = 0) in vec3 in_Position;

uniform mat4 u_ViewProjection;
uniform mat4 u_ModelTransform;

void main() {
	vec4 worldLocation = u_ModelTransform * vec4(in_Position, 1.0);
	gl_Position        = u_ViewProjection * worldLocation;
}
