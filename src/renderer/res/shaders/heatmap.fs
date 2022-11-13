#version 440

in vec2 frag_UVCoordinates;
in vec2 frag_Edgeness;
in vec2 frag_SideUVCoordinates;
out vec4 out_Color;

uniform vec2 u_TemperatureRange;
uniform vec2 u_BottomTemperatureRange;
uniform vec2 u_TopTemperatureRange;
uniform mat2x3 u_ColorRange;
uniform bool u_IsStratified;
uniform float u_StratifiedStep;

uniform vec3 u_edgeColor;
uniform bool u_IsTransparentHeatmap;

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

// leaved for the future, when edges will be drawn itself
float GetSideTemp() {
	float botTemp = GetBotTemp();
	float topTemp = GetTopTemp();
	return mix(botTemp, topTemp, frag_SideUVCoordinates.x);
}

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

bool CheckSingleCoordForEdge(float coord, float delta) {
	return abs(coord) < delta || abs(coord - 1.0f) < delta;
}

bool CheckForEdge(vec2 squarePosition) {
	const float delta = 0.01;
	return CheckSingleCoordForEdge(squarePosition.x, delta) ||
	       CheckSingleCoordForEdge(squarePosition.y, delta);
}

void main() {
	const float eps = 0.001f;
	// Top heatmap
	if (abs(frag_SideUVCoordinates.x - 1.0f) < eps) {
		if (CheckForEdge(frag_Edgeness)) {
			out_Color = vec4(u_edgeColor, 1.0f);
			return;
		}
		out_Color = GetColorByTemp(GetTopTemp());
	}
	// Bottom heatmap
	else if (frag_SideUVCoordinates.x < eps) {
		if (CheckForEdge(frag_Edgeness)) {
			out_Color = vec4(u_edgeColor, 1.0f);
			return;
		}
		out_Color = GetColorByTemp(GetBotTemp());
	}
	// Side heatmap
	else {
		if (CheckForEdge(frag_SideUVCoordinates)) {
			out_Color = vec4(u_edgeColor, 1.0f);
			return;
		}
		out_Color = GetColorByTemp(GetSideTemp());
	}
}
