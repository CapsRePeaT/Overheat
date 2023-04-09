#pragma once

#include <memory>
#include <vector>

#include "glm/glm.hpp"
#include "renderer/vertexarray.h"
#include "text/font.h"
#include "text/text2d.h"
#include "renderer/drawable.h"

namespace renderer {

class TemperatureBar : public Drawable {
 public:
	using Labels = std::vector<std::shared_ptr<Text2D>>;
	TemperatureBar(glm::vec2 size, glm::vec2 position, std::shared_ptr<Font> font,
	               float min_temp = 0, float max_temp = 0);
	~TemperatureBar();

	void SetTemperatureRange(float min, float max);
	void SetColors(glm::vec3 min, glm::vec3 max);

	[[nodiscard]] const VertexArray& vertex_array() const { return *vao_; }
	[[nodiscard]] Labels& labels() { return labels_; }
	[[nodiscard]] std::pair<float, float> temperature_range() {
		return temperature_range_;
	}
	[[nodiscard]] Font& font() { return *font_; }

	bool SetContextForDraw(RendererContext& ctx) override;

 private:
	struct Impl;
	std::unique_ptr<VertexArray> InitVao();
	Labels InitLabels();
	void InitMaterial();

	glm::vec2 size_;
	glm::vec2 position_;
	std::unique_ptr<VertexArray> vao_;
	std::shared_ptr<Font> font_;
	std::pair<float, float> temperature_range_;
	Labels labels_;
	std::unique_ptr<Impl> impl_;
};

}  // namespace renderer
