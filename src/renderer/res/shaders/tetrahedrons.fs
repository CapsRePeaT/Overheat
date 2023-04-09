#version 440

in float frag_Temperature;
out vec4 out_Color;

uniform vec2 u_TemperatureRange;
uniform vec3 u_Colors[5];
uniform bool u_IsStratified;
uniform float u_StratifiedStep;

uniform bool u_IsTransparentHeatmap;

// vec4 GetStratifiedColor(float temp) {
// 	float minTemp = u_TemperatureRange[0];
// 	float maxTemp = u_TemperatureRange[1];
// 	temp          = u_StratifiedStep * floor(temp / u_StratifiedStep);

// 	float factor = smoothstep(minTemp, maxTemp, max(temp, minTemp));
// 	return vec4(mix(u_ColorRange[0], u_ColorRange[1], factor), 1.0f);
// }

vec4 GetColorByTemp(float temp) {
	if (u_IsTransparentHeatmap)
		return vec4(0.f, 0.f, 0.f, 0.0f);
	
	float factor = smoothstep(u_TemperatureRange[0], u_TemperatureRange[1], temp);
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
	return vec4(mix(u_Colors[workingColorsIds[0]], u_Colors[workingColorsIds[1]], new_factor), 1.0);
}

void main() {
    out_Color = GetColorByTemp(frag_Temperature);
}
