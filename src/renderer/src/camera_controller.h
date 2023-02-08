#pragma once

#include <glm/ext/vector_float3.hpp>

#include <memory>

#include "constants.h"
#include "renderer/i_camera.h"
#include "renderer/scene_object.h"

namespace renderer {

template <std::three_way_comparable T>
class BoundedValue {
 public:
	constexpr BoundedValue(T value, T min, T max) : min_(min), max_(max) {
		SetValue(value);
	}
	constexpr inline void SetValue(T value) {
		if (value < min_)
			value = min_;
		else if (value > max_)
			value = max_;
		value_ = value;
	}
	constexpr operator T() const {  // NOLINT(google-explicit-constructor)
		return value_;
	}

	constexpr inline BoundedValue<T>& operator=(T value) {
		SetValue(value);
		return *this;
	}

	[[nodiscard]] constexpr T min() const { return min_; }
	[[nodiscard]] constexpr T max() const { return max_; }

 private:
	T value_;
	T min_;
	T max_;
};

// TODO: Hide scale operations
class SphericalCameraController : public SceneObject {
 public:
	explicit SphericalCameraController(std::unique_ptr<ICamera> camera,
	                                   float radius = 1.0f, float phi = 0.0f,
	                                   float theta = 0.0f)
			: camera_(std::move(camera)),
				radius_(radius, initial_radius.min(), initial_radius.max()),
				phi_(phi, initial_phi.min(), initial_phi.max()),
				theta_(theta, initial_theta.min(), initial_theta.max()) {
		ReplaceCamera();
	};

	void SetPosition(glm::vec3 position) override {
		SceneObject::SetPosition(position);
		ReplaceCamera();
	}

	void SetRadius(float radius);
	inline void AddRadius(float delta) { SetRadius(radius_ + delta); }
	[[nodiscard]] inline float radius() const { return radius_; }

	void SetLongitude(float phi);
	inline void AddLongitude(float delta) { SetLongitude(phi_ + delta); }
	[[nodiscard]] inline float phi() const { return phi_; }

	void SetLatitude(float theta);
	inline void AddLatitude(float delta) { SetLatitude(theta_ + delta); }
	[[nodiscard]] inline float latitude() const { return theta_; }

	inline void SetCamera(std::unique_ptr<ICamera> camera) {
		camera_ = std::move(camera);
	}
	[[nodiscard]] inline const ICamera& camera() const { return *camera_; }
	inline void SetCameraAspectRatio(float ratio) {
		camera_->SetAspectRatio(ratio);
	}
	inline void SetCameraScreenBounds(const int width, const int height) { 
		camera_->SetScreenBounds(width, height); }
	inline void Zoom(float zoom) { camera_->Zoom(zoom); }

 private:
	using BFloat = BoundedValue<float>;
	void ReplaceCamera();

	static constexpr float pi  = glm::pi<float>();
	static constexpr float pi2 = glm::pi<float>() * 2.0f;

	static constexpr auto initial_radius = BFloat(1.0f, 0.0f, 1000.0f);
	static constexpr auto initial_phi    = BFloat(0.0f, 0.0f, pi2);
	static constexpr auto initial_theta  = BFloat(0.0f, -pi * 0.49, pi * 0.49);

	std::unique_ptr<ICamera> camera_;
	BFloat radius_ = initial_radius;
	BFloat phi_    = initial_phi;
	BFloat theta_  = initial_theta;
};

}  // namespace renderer
