#version 430

layout (location = 0) in vec3 in_Positions;
layout (location = 1) in vec3 in_Color;
out vec3 frag_Color;

uniform mat4 u_ViewProjection;
uniform mat4 u_ModelTransform;

void main() {
  gl_Position = u_ViewProjection * u_ModelTransform * vec4(in_Positions, 1.0);
  frag_Color = in_Color;
}
