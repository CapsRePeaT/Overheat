#pragma once

#include <memory>
#include <vector>

#include "glm/glm.hpp"
#include "renderer/vertexarray.h"
#include "text/font.h"
#include "text/text2d.h"

namespace renderer {

class TemperatureBar {
 public:
	using Labels = std::vector<std::shared_ptr<Text2D>>;
	TemperatureBar(glm::vec2 size, glm::vec2 position, std::shared_ptr<Font> font,
	               float min_temp = 0, float max_temp = 0);

	void SetTemperatureRange(float min, float max);

	[[nodiscard]] const VertexArray& vertex_array() const { return *vao_; }
	[[nodiscard]] Labels& labels() { return labels_; }
	[[nodiscard]] std::pair<float, float> temperature_range() {
		return temperature_range_;
	}

 private:
	std::unique_ptr<VertexArray> InitVao();
	Labels InitLabels();

	glm::vec2 size_;
	glm::vec2 position_;
	std::unique_ptr<VertexArray> vao_;
	std::shared_ptr<Font> font_;
	std::pair<float, float> temperature_range_;
	Labels labels_;
};

}  // namespace renderer
