#version 440

in float frag_Temperature;
out vec4 out_Color;

uniform vec2 u_TemperatureRange;
uniform mat2x3 u_ColorRange;
uniform bool u_IsStratified;
uniform float u_StratifiedStep;

uniform bool u_IsTransparentHeatmap;

vec4 GetStratifiedColor(float temp) {
	float minTemp = u_TemperatureRange[0];
	float maxTemp = u_TemperatureRange[1];
	temp          = u_StratifiedStep * floor(temp / u_StratifiedStep);

	float factor = smoothstep(minTemp, maxTemp, max(temp, minTemp));
	return vec4(mix(u_ColorRange[0], u_ColorRange[1], factor), 1.0f);
}

vec4 GetColorByTemp(float temp) {
	if (u_IsTransparentHeatmap)
		return vec4(0.f, 0.f, 0.f, 0.0f);
	
	float factor = smoothstep(u_TemperatureRange[0], u_TemperatureRange[1], temp);
	if (u_IsStratified)
		return GetStratifiedColor(temp);
	return vec4(mix(u_ColorRange[0], u_ColorRange[1], factor), 1.0f);
}

void main() {
    out_Color = GetColorByTemp(frag_Temperature);
}
