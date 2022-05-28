#pragma once

#include <glm/ext/quaternion_float.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace renderer {

namespace consts {

/* Scalar constants */
// General constants
constexpr int buffer_count_one          = 1;
constexpr int vertex_array_count_one    = 1;
constexpr int error_message_buffer_size = 4096;

/* Vectors */
constexpr glm::vec3 vec3_0 = {0.0f, 0.0f, 0.0f};

constexpr glm::vec3 vec3_x = {1.0f, 0.0f, 0.0f};
constexpr glm::vec3 vec3_y = {0.0f, 1.0f, 0.0f};
constexpr glm::vec3 vec3_z = {0.0f, 0.0f, 1.0f};
// Radius vectors in homogeneous coordinate system
constexpr glm::vec4 vec4_x = {1.0f, 0.0f, 0.0f, 1.0f};
constexpr glm::vec4 vec4_y = {0.0f, 1.0f, 0.0f, 1.0f};
constexpr glm::vec4 vec4_z = {0.0f, 0.0f, 1.0f, 1.0f};
// Direction vectors in homogeneous coordinate system
constexpr glm::vec4 vec4_x_v = {1.0f, 0.0f, 0.0f, 0.0f};
constexpr glm::vec4 vec4_y_v = {0.0f, 1.0f, 0.0f, 0.0f};
constexpr glm::vec4 vec4_z_v = {0.0f, 0.0f, 1.0f, 0.0f};

constexpr glm::vec3 color_not_selected    = vec3_0;
constexpr glm::vec3 color_selected        = {1.0f, 0.66f, 0.0f};
constexpr glm::vec3 color_active_selected = {1.0f, 0.33f, 0.0f};

namespace init {
// Camera
constexpr float zoom           = 15.0f;
constexpr float near_plane     = 1.0f;
constexpr float far_plane      = 2000.0f;
constexpr auto near_far_bounds = std::make_pair(near_plane, far_plane);
// Renderer
constexpr glm::vec4 clear_color = {0.49, 0.49, 0.49, 1};
// Objects
constexpr glm::vec3 position = {0.0f, 0.0f, 0.0f};
constexpr glm::vec3 scale    = {1.0f, 1.0f, 1.0f};
constexpr glm::quat rotation = {1.0f, 0.0f, 0.0f, 0.0f};
}  // namespace init

}  // namespace consts

}  // namespace renderer
