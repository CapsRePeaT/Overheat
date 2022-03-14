#version 430

in vec3 frag_Color;
out vec4 out_Color;

uniform float u_Alpha;

void main() {
  out_Color = vec4(frag_Color, u_Alpha);
}
