#pragma once

#include <glad/glad.h>

#include <filesystem>
#include <glm/gtc/matrix_transform.hpp>
#include <string>

class ShaderProgram {
 public:
	using Path = std::filesystem::path;
	ShaderProgram(const Path& vertex_shader, const Path& fragment_shader);
	ShaderProgram(ShaderProgram&& other) noexcept;
	ShaderProgram& operator=(ShaderProgram&& other) noexcept;

	ShaderProgram() = delete;
	ShaderProgram(const ShaderProgram&) = delete;
	ShaderProgram& operator=(const ShaderProgram&) = delete;

	~ShaderProgram();

	inline void Use() const {
		if (id_ != using_id_) {
			glUseProgram(id_);
			using_id_ = id_;
		}
	}
	[[nodiscard]] bool isCompiled() const noexcept { return is_compiled_; }

	void SetInt(const std::string& name, int32_t value) const;
	void SetFloat(const std::string& name, float value) const;
	void SetMat4(const std::string& name, const glm::mat4& value) const;
	void SetVec3(const std::string& name, const glm::vec3& value) const;

 private:
	[[nodiscard]] int32_t getUniformLocation(const std::string& name) const;
	static bool createShader(const std::string& source, GLenum shader_type,
	                         uint32_t& shader_id);

	bool is_compiled_ = false;
	uint32_t id_ = 0;
	static uint32_t using_id_;
};
