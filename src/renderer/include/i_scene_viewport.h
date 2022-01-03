#pragma once

#include <memory>

#include "common.h"
#include "core.h"

class ISceneViewport {
 public:
	enum class API {
		None,
		OpenGL,
		Vulkan,
		D3D,
	};

	using TemperatureRange = std::pair<float, float>;
	using Color = std::array<float, 3>;
	using ColorRange = std::pair<Color, Color>;
	virtual ~ISceneViewport() = default;
	static std::unique_ptr<ISceneViewport> Create(API api);
	// Initializes OpenGL functions and resources.
	// Needs to be called with active OpenGL context.
	virtual void Initialize(int w, int h) = 0;
	// Clears all of the hardware resources to gracefully shutdown
	// renderer API context.
	// Must be called in overriden dtors of this interface!
	virtual void ClearResources() = 0;
	virtual void RenderFrame() = 0;
	virtual void Resize(int w, int h) = 0;
	// Removes objects from scene
	virtual void ClearScene() = 0;
	virtual void RenderShapes(const Core::Shapes& shapes) {
		for (const auto& shape : shapes) AddShape(shape);
	}
	virtual void SetTemperatureRange(float min, float max) = 0;
	inline void SetTemperatureRange(TemperatureRange temps) {
		SetTemperatureRange(temps.first, temps.second);
	}
	virtual void SetColorRange(Color min, Color max) = 0;
	inline void SetColorRange(ColorRange colors) {
		SetColorRange(colors.first, colors.second);
	}
	inline void SetMinMaxTemperatureAndColorRanges(TemperatureRange temperatures,
	                                               ColorRange colors) {
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
