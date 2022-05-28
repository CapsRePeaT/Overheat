#version 440

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec2 in_UVCoordinates;
layout(location = 2) in vec2 in_Topness;
out vec2 frag_UVCoordinates;
out vec2 frag_Edgeness;
out vec2 frag_SideUVCoordinates;

uniform mat4 u_ViewProjection;
uniform mat4 u_ModelTransform;
uniform mat2 u_Bounds;

void main() {
	vec4 worldLocation = u_ModelTransform * vec4(in_Position, 1.0);
	gl_Position        = u_ViewProjection * worldLocation;
	vec2 bounds_range  = u_Bounds[1] - u_Bounds[0];
	frag_UVCoordinates = (in_Position.xy - u_Bounds[0]) / bounds_range;
	frag_Edgeness = in_UVCoordinates;
	frag_SideUVCoordinates = in_Topness;
}
