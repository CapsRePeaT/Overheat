#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace consts {

constexpr int buffer_count_one = 1;
constexpr int vertex_array_count_one = 1;
constexpr int error_message_buffer_size = 4096;

constexpr glm::vec3 vec3_x = {1.0f, 0.0f, 0.0f};
constexpr glm::vec3 vec3_y = {0.0f, 1.0f, 0.0f};
constexpr glm::vec3 vec3_z = {0.0f, 0.0f, 1.0f};

constexpr glm::vec4 vec4_x = {1.0f, 0.0f, 0.0f, 0.0f};
constexpr glm::vec4 vec4_y = {0.0f, 1.0f, 0.0f, 0.0f};
constexpr glm::vec4 vec4_z = {0.0f, 0.0f, 1.0f, 0.0f};

}  // namespace consts
