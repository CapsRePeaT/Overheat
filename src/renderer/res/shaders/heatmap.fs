#version 430

in vec2 frag_UVCoordinates;
out vec4 out_Color;

uniform vec2 u_TemperatureRange;
uniform mat2x3 u_ColorRange;

float sqr(float x) { return x*x; }

float parabolicDome(float x, float y) {
	float maxTemp = 400;
	float a = 400;
	return maxTemp - a * (sqr(x-0.5f) + sqr(y-0.5f));

}

float parabolicDome(vec2 xy) {
	return parabolicDome(xy.x, xy.y);
}

void main() {
	float temp = parabolicDome(frag_UVCoordinates);

	float factor = smoothstep(u_TemperatureRange[0], u_TemperatureRange[1], temp);

	out_Color = vec4(mix(u_ColorRange[0], u_ColorRange[1], sqr(factor)), 1.0f);
}
