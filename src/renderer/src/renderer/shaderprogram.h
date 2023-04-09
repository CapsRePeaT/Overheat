#pragma once

#include <glm/gtc/matrix_transform.hpp>

#include <filesystem>
#include <string>

namespace renderer {

// TODO: abstract shader and make ShaderLibrary
class ShaderProgram {
 public:
	using Path = std::filesystem::path;
	ShaderProgram(const Path& vertex_shader_path,
	              const Path& fragment_shader_path);
	ShaderProgram(ShaderProgram&& other) noexcept;
	ShaderProgram& operator=(ShaderProgram&& other) noexcept;
	~ShaderProgram();
	ShaderProgram(const ShaderProgram&) = delete;
	ShaderProgram& operator=(const ShaderProgram&) = delete;

	bool Load(const Path& vertex_shader_path, const Path& fragment_shader_path);
	void Use() const;
	static void Unuse();
	[[nodiscard]] bool isUsing() const { return id_ == using_id_; }
	[[nodiscard]] bool isCompiled() const noexcept { return is_compiled_; }
	void SetInt(const char* name, int32_t value) const;
	void SetBool(const char* name, bool value) const;
	void SetFloat(const char* name, float value) const;
	void SetFloats(const char* name, const float* values, int count) const;
	void SetVec2(const char* name, glm::vec2 value) const;
	void SetVec3(const char* name, glm::vec3 value) const;
	void SetVec3v(const char* name, const glm::vec3* values, int count) const;
	void SetMat4(const char* name, const glm::mat4& value) const;
	void SetMat2x3(const char* name, glm::mat2x3 value) const;
	void SetMat2(const char* name, glm::mat2 value) const;

 private:
	[[nodiscard]] int32_t getUniformLocation(const char* name) const;
	bool is_compiled_ = false;
	uint32_t id_ = 0;
	static uint32_t using_id_;
};

}  // namespace renderer
