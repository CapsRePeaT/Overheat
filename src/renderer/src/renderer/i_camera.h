#pragma once

#include <glm/ext/quaternion_geometric.hpp>
#include <glm/ext/vector_float3.hpp>
#include "renderer/scene_object.h"

class ICamera : public SceneObject {
 public:
	inline void SetScale(glm::vec3 /*scale*/) final {}

	[[nodiscard]] glm::mat4 transform() const override {
		const glm::mat4 rot = glm::toMat4(rotation());
		return glm::translate(glm::mat4(1.0f), position()) * rot;
	}

	virtual void LookAt(glm::vec3 point_to_view) = 0;
	// Zoom view relative to current zoom level
	virtual void Zoom(float delta) = 0;
	[[nodiscard]] virtual float zoom_level() const = 0;
	virtual void SetAspectRatio(float ratio) = 0;
	[[nodiscard]] virtual float aspect_ratio() const = 0;
	
	[[nodiscard]] virtual const glm::mat4& viewMatrix() const = 0;
	[[nodiscard]] virtual const glm::mat4& projectionMatrix() const = 0;
	[[nodiscard]] virtual const glm::mat4& viewProjectionMatrix() const = 0;
};
