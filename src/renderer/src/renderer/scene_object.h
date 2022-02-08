#pragma once

#include <glm/gtx/quaternion.hpp>

#include "constants.h"

class SceneObject {
 public:
	explicit SceneObject(glm::vec3 position = consts::initial_position,
	                     glm::vec3 scale = consts::initial_scale,
	                     glm::quat rotation = consts::initial_rotation)
			: position_(position), scale_(scale), rotation_(rotation) {}

	// Setters and getters of common transformation attributes of any scene object
	virtual inline void SetPosition(glm::vec3 position) { position_ = position; }
	[[nodiscard]] virtual inline glm::vec3 position() const { return position_; }
	virtual inline void SetScale(glm::vec3 scale) { scale_ = scale; }
	[[nodiscard]] virtual inline glm::vec3 scale() const { return scale_; }
	virtual inline void SetRotation(glm::quat rotation) { rotation_ = rotation; }
	[[nodiscard]] virtual inline glm::quat rotation() const { return rotation_; }
	[[nodiscard]] virtual inline glm::mat4 transform() const {
		const glm::mat4 rot = glm::toMat4(rotation());
		return glm::translate(glm::mat4(1.0f), position_) * rot *
		       glm::scale(glm::mat4(1.0f), scale_);
	}

	[[nodiscard]] inline glm::vec3 forwardVector() const {
		return glm::rotate(rotation(), consts::vec3_x);
	}
	[[nodiscard]] inline glm::vec3 rightVector() const {
		return glm::rotate(rotation(), consts::vec3_y);
	}
	[[nodiscard]] inline glm::vec3 upVector() const {
		return glm::rotate(rotation(), consts::vec3_z);
	}
	[[nodiscard]] virtual inline float roll() const {
		return glm::eulerAngles(rotation_).x;
	}
	[[nodiscard]] virtual inline float pitch() const {
		return glm::eulerAngles(rotation_).y;
	}
	[[nodiscard]] virtual inline float yaw() const {
		return glm::eulerAngles(rotation_).z;
	}

	// Modifiers of transformation
	inline void Translate(glm::vec3 translation) {
		SetPosition(position_ + translation);
	}
	inline void ApplyScale(glm::vec3 scale) { SetScale(scale_ * scale); }
	inline void ApplyScale(float scale) { SetScale(scale_ * scale); }
	inline void Rotate(float angle, glm::vec3 axis) {
		SetRotation(glm::rotate(rotation(), angle, axis));
	}
	inline void Rotate(glm::quat rotation) {
		Rotate(glm::angle(rotation), glm::axis(rotation));
	}
	inline void AddRoll(float angle) { Rotate(angle, consts::vec4_x_v); }
	inline void AddPitch(float angle) { Rotate(angle, consts::vec4_y_v); }
	inline void AddYaw(float angle) { Rotate(angle, consts::vec4_z_v); }

 private:
	glm::vec3 position_ = consts::initial_position;
	glm::vec3 scale_ = consts::initial_scale;
	glm::quat rotation_ = consts::initial_rotation;
};
