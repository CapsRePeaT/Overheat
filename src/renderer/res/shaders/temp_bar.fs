#version 440

in vec2 frag_UVCoordinates;
out vec4 out_Color;

uniform vec3 u_Colors[5];


void main() {
    float factor = smoothstep(0.0f, 1.0f, frag_UVCoordinates[1]);
	float factors[5]         = float[](0.0, 0.15, 0.5, 0.85, 1.0);
	int workingFactorsIds[2] = int[](3, 4);
	int workingColorsIds[2]  = int[](3, 4);
	if (factor < factors[1]) {
		workingColorsIds = int[](0, 1);
		workingFactorsIds = int[](0, 1);
	} else if (factor < factors[2]) {
		workingColorsIds = int[](1, 2);
		workingFactorsIds = int[](1, 2);
	} else if (factor < factors[3]) {
		workingColorsIds = int[](2, 3);
		workingFactorsIds = int[](2, 3);
	}
	float new_factor = smoothstep(factors[workingFactorsIds[0]], factors[workingFactorsIds[1]], factor);
	out_Color = vec4(mix(u_Colors[workingColorsIds[0]], u_Colors[workingColorsIds[1]], new_factor), 1.0);
    // out_Color = vec4(frag_UVCoordinates, 0, 1);
}
