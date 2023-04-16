#version 440

out vec4 out_Color;

uniform vec3 u_edgeColor;

void main() {
    out_Color = vec4(u_edgeColor, 1.0);
}
