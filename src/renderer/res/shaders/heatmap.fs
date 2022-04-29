#version 440

in vec2 frag_UVCoordinates;
in float frag_Topness;
out vec4 out_Color;

uniform vec2 u_TemperatureRange;
uniform vec2 u_BottomTemperatureRange;
uniform vec2 u_TopTemperatureRange;
uniform mat2x3 u_ColorRange;

uniform sampler2D u_BottomHeatmap;
uniform sampler2D u_TopHeatmap;

float GetTopOrBottomTemp(sampler2D heatmap, vec2 heatmapTempRange) {
	float heatmap_factor = texture(heatmap, frag_UVCoordinates).r;
	return mix(heatmapTempRange[0], heatmapTempRange[1], heatmap_factor);
}

float GetBotTemp() {
	return GetTopOrBottomTemp(u_BottomHeatmap, u_BottomTemperatureRange);
}

float GetTopTemp() {
	return GetTopOrBottomTemp(u_TopHeatmap, u_TopTemperatureRange);
}

float GetSideTemp() {
	float botTemp = GetBotTemp();
	float topTemp = GetTopTemp();
	return mix(botTemp, topTemp, frag_Topness);
}

vec4 GetColorByTemp(float temp) {
	float factor = smoothstep(u_TemperatureRange[0], u_TemperatureRange[1], temp);
	return vec4(mix(u_ColorRange[0], u_ColorRange[1], factor), 1.0f);
}

void main() {
	const float eps = 0.001f;
	// Top heatmap
	if (abs(frag_Topness - 1.0f) < eps) {
		out_Color = GetColorByTemp(GetTopTemp());
	}
	// Bottom heatmap
	else if (frag_Topness < eps) {
		out_Color = GetColorByTemp(GetBotTemp());
	}
	// Side heatmap
	else {
		out_Color = GetColorByTemp(GetSideTemp());
	}
}
