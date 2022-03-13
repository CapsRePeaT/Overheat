#include "shaderprogram.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <spdlog/spdlog.h>

#include <array>
#include <fstream>
#include <sstream>

#include "constants.h"

namespace renderer {

uint32_t ShaderProgram::using_id_;
bool CreateShader(const ShaderProgram::Path& source, GLenum shader_type,
                  uint32_t& shader_id);

ShaderProgram::ShaderProgram(const Path& vertex_shader_path,
                             const Path& fragment_shader_path)
		: is_compiled_(false) {
	Load(vertex_shader_path, fragment_shader_path);
}

ShaderProgram::~ShaderProgram() {
	if (isUsing())
		Unuse();
	glDeleteProgram(id_);
	id_ = 0;
}

ShaderProgram::ShaderProgram(ShaderProgram&& other) noexcept {
	*this = std::move(other);
}

ShaderProgram& ShaderProgram::operator=(ShaderProgram&& other) noexcept {
	if (this == &other)
		return *this;
	// Delete owned shader
	glDeleteProgram(id_);
	// Assign moving shader data
	id_ = other.id_;
	is_compiled_ = other.is_compiled_;
	// Invalidate moving shader
	other.id_ = 0;
	other.is_compiled_ = false;
	return *this;
}

template <GLenum ptype>
[[nodiscard]] bool GetCompileLinkSuccess(const int32_t id) {
	static_assert(ptype == GL_LINK_STATUS || ptype == GL_COMPILE_STATUS,
	              "Call for GetCompileLinkSuccess is permitted with only  with"
	              "GL_LINK_STATUS or GL_COMPILE_STATUS");
	int32_t success = 0;
	if constexpr (ptype == GL_LINK_STATUS)
		glGetProgramiv(id, ptype, &success);
	else if constexpr (ptype == GL_COMPILE_STATUS)
		glGetShaderiv(id, ptype, &success);
	return success;
}

template <GLenum ptype>
[[nodiscard]] std::string GetCompileLinkInfoLog(const int32_t id) {
	static_assert(ptype == GL_LINK_STATUS || ptype == GL_COMPILE_STATUS,
	              "Call for GetCompileLinkInfoLog is permitted with only  with"
	              "GL_LINK_STATUS or GL_COMPILE_STATUS");
	std::array<GLchar, consts::error_message_buffer_size> info_log{};
	if constexpr (ptype == GL_LINK_STATUS)
		glGetProgramInfoLog(id, info_log.size(), nullptr, info_log.data());
	else if constexpr (ptype == GL_COMPILE_STATUS)
		glGetShaderInfoLog(id, info_log.size(), nullptr, info_log.data());
	return info_log.data();
}

template <GLenum ptype, typename... Args>
bool CheckSuccessAndLogError(const int32_t id,
                             fmt::format_string<Args...> msg_template,
                             Args&&... args) {
	static_assert(ptype == GL_LINK_STATUS || ptype == GL_COMPILE_STATUS,
	              "Call for CheckSuccess is permitted with only  with"
	              "GL_LINK_STATUS or GL_COMPILE_STATUS");
	const bool success = GetCompileLinkSuccess<ptype>(id);
	if (!success) {
		const auto info_log = GetCompileLinkInfoLog<ptype>(id);
		spdlog::error(msg_template, std::forward<Args>(args)...);
		spdlog::error(info_log);
	}
	return success;
}

bool ShaderProgram::Load(const Path& vertex_path, const Path& fragment_path) {
	// TODO: find out if we need explicitly check errors with debug callback
	// Create vertex and fragment shaders
	uint32_t vs;
	uint32_t fs;
	bool shaders_compiled = true;
	shaders_compiled &= CreateShader(vertex_path, GL_VERTEX_SHADER, vs);
	shaders_compiled &= CreateShader(fragment_path, GL_FRAGMENT_SHADER, fs);
	if (!shaders_compiled)
		return is_compiled_ = false;
	// Create shader program, link and delete the shaders
	id_ = glCreateProgram();
	glAttachShader(id_, vs);
	glAttachShader(id_, fs);
	glLinkProgram(id_);
	glDeleteShader(vs);
	glDeleteShader(fs);
	return is_compiled_ = CheckSuccessAndLogError<GL_LINK_STATUS>(
						 id_, "SHADER: Link time error:");
}

bool CreateShader(const ShaderProgram::Path& file_path,
                  const GLenum shader_type, uint32_t& shader_id) {
	// Open and read file into code_pointer
	std::ifstream file_stream;
	file_stream.open(file_path, std::ios::in | std::ios::binary);
	if (!file_stream.is_open()) {
		spdlog::error("SHADER: Failed to open file: {}", file_path.string());
		return false;
	}
	std::stringstream buff;
	buff << file_stream.rdbuf();
	const auto code_string = buff.str();
	const std::array<const char*, 1> source_codes = {code_string.c_str()};
	// Compile read code
	shader_id = glCreateShader(shader_type);
	glShaderSource(shader_id, source_codes.size(), source_codes.data(),
	               /*length=*/nullptr);
	glCompileShader(shader_id);
	bool success = CheckSuccessAndLogError<GL_COMPILE_STATUS>(
			shader_id, "SHADER: Compile time error in {}", file_path.string());
	if (!success)
		glDeleteShader(shader_id);
	return success;
}

// TODO: cache uniform locations and think about refactoring
int32_t ShaderProgram::getUniformLocation(const char* name) const {
	return glGetUniformLocation(id_, name);
}

void ShaderProgram::SetInt(const char* name, const int32_t value) const {
	glUniform1i(getUniformLocation(name), value);
}

void ShaderProgram::SetFloat(const char* name, const float value) const {
	glUniform1f(getUniformLocation(name), value);
}
void ShaderProgram::SetVec2(const char* name, const glm::vec2 value) const {
	glUniform2f(getUniformLocation(name), value.x, value.y);
}
void ShaderProgram::SetVec3(const char* name, const glm::vec3 value) const {
	glUniform3f(getUniformLocation(name), value.x, value.y, value.z);
}
void ShaderProgram::SetMat4(const char* name, const glm::mat4& value) const {
	glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE,
	                   glm::value_ptr(value));
}
void ShaderProgram::SetMat2x3(const char* name, const glm::mat2x3 value) const {
	glUniformMatrix2x3fv(getUniformLocation(name), 1, GL_FALSE,
	                     glm::value_ptr(value));
}

void ShaderProgram::Use() const {
	if (!is_compiled_)
		spdlog::warn("ShaderProgram (id = {}) is used but not compiled", id_);
	if (id_ != using_id_) {
		glUseProgram(id_);
		using_id_ = id_;
	}
}

void ShaderProgram::Unuse() {
	glUseProgram(0);
	using_id_ = 0;
}

}  // namespace renderer
