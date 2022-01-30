#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class SceneObject {
 public:
	virtual inline void SetPosition(glm::vec3 position) { position_ = position; }
	[[nodiscard]] virtual inline glm::vec3 position() const { return position_; }
	virtual inline void SetScale(glm::vec3 scale) { scale_ = scale; }
	[[nodiscard]] virtual inline glm::vec3 scale() const { return scale_; }
	virtual inline void SetRotation(glm::quat rotation) { rotation_ = rotation; }
	[[nodiscard]] virtual inline glm::quat rotation() const { return rotation_; }
	[[nodiscard]] virtual inline glm::mat4 transform() const {
		const glm::mat4 rot = glm::mat4_cast(rotation_);
		return glm::translate(glm::mat4(1.0f), position_) * rot *
		       glm::scale(glm::mat4(1.0f), scale_);
	}
	
	virtual inline void Translate(glm::vec3 translation) {
		position_ += translation;
	}
	virtual inline void ApplyScale(glm::vec3 scale) {
		scale_ *= scale;
	}
	virtual inline void ApplyScale(float scale) {
		scale_ *= scale;
	}
	virtual inline void Rotate(float angle, glm::vec3 axis) {
		rotation_ = glm::rotate(rotation_, angle, axis);
	}
	virtual inline void Rotate(glm::quat rotation) {
		Rotate(glm::angle(rotation), glm::axis(rotation));
	}

 protected:
	glm::vec3 position_ = {0.0f, 0.0f, 0.0f};
	glm::vec3 scale_ = {1.0f, 1.0f, 1.0f};
	glm::quat rotation_ = glm::quat({0.0f, 0.0f, 0.0f});
};
