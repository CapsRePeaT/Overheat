#version 440

in vec2 frag_UVCoordinates;
out vec4 out_Color;

uniform mat2x3 u_ColorRange;

void main() {
    float factor = smoothstep(0.0f, 1.0f, frag_UVCoordinates[1]);
    out_Color = vec4(mix(u_ColorRange[0], u_ColorRange[1], frag_UVCoordinates[1]), 1.0f);
    // out_Color = vec4(frag_UVCoordinates, 0, 1);
}