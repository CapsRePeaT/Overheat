#include "shaderprogram.h"

#include <glad/glad.h>
#include <spdlog/spdlog.h>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <sstream>

constexpr int kGBufferSize = 4096;

uint32_t ShaderProgram::using_id_;
bool CreateShader(const ShaderProgram::Path& source, GLenum shader_type, uint32_t& shader_id);

ShaderProgram::ShaderProgram(const Path& vertex_shader_path,
                             const Path& fragment_shader_path)
		: is_compiled_(true) {
	Load(vertex_shader_path, fragment_shader_path);
}

ShaderProgram::~ShaderProgram() {
	if (isUsing()) Unuse();
	glDeleteProgram(id_);
	id_ = 0;
}

ShaderProgram::ShaderProgram(ShaderProgram&& other) noexcept {
	*this = std::move(other);
}

ShaderProgram& ShaderProgram::operator=(ShaderProgram&& other) noexcept {
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

bool ShaderProgram::Load(const Path& vertex_shader_path,
                         const Path& fragment_shader_path) {
	// TODO: find out if we need explicitly check errors with debug callback
	// Create vertex and fragment shaders
	uint32_t vs = 0;
	uint32_t fs = 0;
	bool is_vertex_compiled =
			CreateShader(vertex_shader_path, GL_VERTEX_SHADER, vs);
	bool is_fragment_compiled =
			CreateShader(fragment_shader_path, GL_FRAGMENT_SHADER, fs);
	// If one of the shaders didn't compile, delete them and return
	if (!(is_vertex_compiled && is_fragment_compiled)) {
		if (is_vertex_compiled) glDeleteShader(vs);
		if (is_fragment_compiled) glDeleteShader(fs);
		return is_compiled_ = false;
	}
	// Create shader program and link the shaders
	id_ = glCreateProgram();
	glAttachShader(id_, vs);
	glAttachShader(id_, fs);
	glLinkProgram(id_);
	// Check link errors
	int32_t success = 0;
	glGetProgramiv(id_, GL_LINK_STATUS, &success);
	if (success == 0) {
		std::array<GLchar, kGBufferSize> info_log{};
		glGetProgramInfoLog(id_, kGBufferSize, nullptr, info_log.data());
		spdlog::error("SHADER: Link time error in:\n{}", info_log.data());
		is_compiled_ = false;
	}

	glDeleteShader(vs);
	glDeleteShader(fs);

	return is_compiled_;
}

bool CreateShader(const ShaderProgram::Path& source, GLenum shader_type,
                  uint32_t& shader_id) {
	std::ifstream file_stream;
	file_stream.open(source, std::ios::in | std::ios::binary);
	if (!file_stream.is_open()) {
		spdlog::error("SHADER: Failed to open file: {}", source);
		return false;
	}
	std::stringstream buff;
	buff << file_stream.rdbuf();
	std::string code_string = buff.str();
	const char* code_pointer = code_string.c_str();

	shader_id = glCreateShader(shader_type);
	glShaderSource(shader_id, 1, &code_pointer, nullptr);
	glCompileShader(shader_id);

	int32_t success = 0;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
	if (success == 0) {
		std::array<GLchar, kGBufferSize> info_log{};
		glGetShaderInfoLog(shader_id, kGBufferSize, nullptr, info_log.data());
		spdlog::error("SHADER: Compile time error in {}:\n{}", source,
		              info_log.data());
		return false;
	}

	return true;
}

// TODO: cache uniform locations and think about refactoring
int32_t ShaderProgram::getUniformLocation(const std::string& name) const {
	return glGetUniformLocation(id_, name.c_str());
}

void ShaderProgram::SetInt(const std::string& name, int32_t value) const {
	glUniform1i(getUniformLocation(name), value);
}

void ShaderProgram::SetFloat(const std::string& name, float value) const {
	glUniform1f(getUniformLocation(name), value);
}
void ShaderProgram::SetVec2(const std::string& name,
                            const glm::vec2& value) const {
	glUniform2f(getUniformLocation(name), value.x, value.y);
}
void ShaderProgram::SetVec3(const std::string& name,
                            const glm::vec3& value) const {
	glUniform3f(getUniformLocation(name), value.x, value.y, value.z);
}
void ShaderProgram::SetMat4(const std::string& name,
                            const glm::mat4& value) const {
	glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE,
	                   glm::value_ptr(value));
}
void ShaderProgram::SetMat2x3(const std::string& name,
                              const glm::mat2x3& value) const {
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
