#version 440

in vec2 frag_UVCoordinates;
out vec4 out_Color;

uniform sampler2D u_FontAtlas;

void main() {
    float alpha = texture(u_FontAtlas, frag_UVCoordinates).r;
    out_Color = vec4(0.0, 0.0, 0.0, alpha);
    // out_Color = vec4(frag_UVCoordinates, 1.0, 1.0);
}
