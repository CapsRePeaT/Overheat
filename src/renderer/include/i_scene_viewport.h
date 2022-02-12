#pragma once

#include <memory>

#include "common.h"
#include "core.h"
#include "scene.h"
#include "vector.h"

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
	static std::unique_ptr<ISceneViewport> Create(API api,
	                                              std::shared_ptr<Scene> scene);
	// Initializes OpenGL functions and resources.
	// Needs to be called with active OpenGL context.
	virtual void Initialize(int w, int h) = 0;
	// Clears all of the hardware resources to gracefully shutdown
	// renderer API context.
	// Must be called in overriden dtors of this interface!
	virtual void ClearResources() = 0;
	virtual void RenderFrame() = 0;
	virtual void Resize(int w, int h) = 0;

	// Moves camera perpendicular to it's forward vector
	// screenPoint - mouse position in screen view (0 in top-left corner)
	// delta - last frame increment of mouse position
	virtual void MoveCamera(Vec2D screenPoint, Vec2D delta) = 0;
	// Rotates camera according to it's controller
	// screenPoint - mouse position in screen view (0 in top-left corner)
	// delta - last frame increment of mouse position
	virtual void RotateCamera(Vec2D screenPoint, Vec2D delta) = 0;
	// Zooms view in and out
	virtual void ZoomView(float delta) = 0;

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
	DrawMode draw_mode_ = DrawMode::Gradient;
	bool is_initialized_ = false;
};
