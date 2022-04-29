#version 440

layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec2 in_UVCoordinates;
layout (location = 2) in float in_Topness;
out vec2 frag_UVCoordinates;
out float frag_Topness;


uniform mat4 u_ViewProjection;
uniform mat4 u_ModelTransform;

void main() {
	vec4 modelLocation = u_ModelTransform * vec4(in_Position, 1.0);
	gl_Position = u_ViewProjection * modelLocation;
	frag_UVCoordinates = modelLocation.xy;
	frag_Topness = in_Topness;
}
