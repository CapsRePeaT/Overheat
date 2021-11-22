#pragma once

#include "common.h"
#include "core.h"

// TODO: rename to SceneViewport
// TODO: make method for retrieving new implementation
class IRenderer {
 public:
	using TemperatureRange = std::pair<float, float>;
	using Color = std::array<float, 3>;
	using ColorRange = std::pair<Color, Color>;
	virtual ~IRenderer() = default;
	// Initializes OpenGL functions and resources.
	// Needs to be called with active OpenGL context.
	virtual void Initialize(const int w, const int h) = 0;
	// Clears all of the hardware resources to gracefully shutdown
	// renderer API context.
	// Must be called in overriden dtors of this interface!
	virtual void ClearResources() = 0;
	virtual void RenderFrame() = 0;
	virtual void Resize(const int w, const int h) = 0;
	// Removes objects from scene
	virtual void ClearScene() = 0;
	virtual void RenderShapes(const Core::Shapes& shapes) {
		for (const auto& shape : shapes) AddShape(shape);
	}
	virtual void SetTemperatureRange(const float min, const float max) = 0;
	inline void SetTemperatureRange(const TemperatureRange& temps) {
		SetTemperatureRange(temps.first, temps.second);
	}
	virtual void SetColorRange(const Color min, const Color max) = 0;
	inline void SetColorRange(const ColorRange& colors) {
		SetColorRange(colors.first, colors.second);
	}
	inline void SetMinMaxTemperatureAndColorRanges(
			const TemperatureRange& temperatures, const ColorRange& colors) {
		SetTemperatureRange(temperatures);
		SetColorRange(colors);
	}
	void SetDrawMode(DrawMode mode) { draw_mode_ = mode; };
	[[nodiscard]] bool is_initialized() const { return is_initialized_; }

 protected:
	virtual void AddShape(const std::shared_ptr<BasicShape>& shape) = 0;
	DrawMode draw_mode_;
	bool is_initialized_ = false;
};
