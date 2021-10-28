#include <shaderprogram.h>

#include <opengllog.h>

#include <glm/gtc/type_ptr.hpp>

using namespace gl; // NOLINT(google-build-using-namespace)

constexpr int g_bufferSize = 4096;

GLuint ShaderProgram::usingId = 0;

ShaderProgram::ShaderProgram(const Path& vertexShader, const Path& fragmentShader) {
	GLuint vs = 0;
	GLuint fs = 0;
	bool isVertexCompiled = _createShader(vertexShader, GL_VERTEX_SHADER, vs);
	bool isFragmentCompiled = _createShader(fragmentShader, GL_FRAGMENT_SHADER, fs);

	m_isCompiled = isVertexCompiled && isFragmentCompiled; // NOLINT(cppcoreguidelines-prefer-member-initializer)

	if (!m_isCompiled) {
		if (isVertexCompiled) glCall<glDeleteShader>(vs);
		if (isFragmentCompiled) glCall<glDeleteShader>(fs);
		return;
	}

	m_id = glCall<glCreateProgram>();
	glCall<glAttachShader>(m_id, vs);
	glCall<glAttachShader>(m_id, fs);
	glCall<glLinkProgram>(m_id);

	GLint success = 0;
	GL_CALL(glGetProgramiv(m_id, GL_LINK_STATUS, &success));
	if (success == 0) {
		std::array<GLchar, g_bufferSize> infoLog{};
		glCall<glGetProgramInfoLog>(m_id, g_bufferSize, nullptr, infoLog.data());
		LOG_ERROR("SHADER: Link time error in:\n{}", infoLog.data());
		m_isCompiled = false;
	}

	glCall<glDeleteShader>(vs);
	glCall<glDeleteShader>(fs);

}

ShaderProgram::~ShaderProgram() {
	glDeleteProgram(m_id);
	usingId = 0;
}

bool ShaderProgram::_createShader(const std::string& source, gl::GLenum shaderType, gl::GLuint& shaderId) {
	const char* code = source.c_str();

	shaderId = glCall<glCreateShader>(shaderType);
	glCall<glShaderSource>(shaderId, 1, &code, nullptr);
	glCall<glCompileShader>(shaderId);

	GLint success = 0;
	GL_CALL(glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success));
	if (success == 0) {
		std::array<GLchar, g_bufferSize> infoLog{};
		glCall<glGetShaderInfoLog>(shaderId, g_bufferSize, nullptr, infoLog.data());
		LOG_ERROR("SHADER: Compile time error in {}\n:\n{}", source, infoLog.data());
		return false;
	}

	return true;
}

ShaderProgram::ShaderProgram(ShaderProgram&& other) noexcept
		: m_id(other.m_id)
		, m_isCompiled(other.m_isCompiled) {
	*this = std::move(other);
}

ShaderProgram& ShaderProgram::operator=(ShaderProgram&& other) noexcept {
	glDeleteProgram(m_id);
	m_id = other.m_id;
	m_isCompiled = other.m_isCompiled;

	other.m_id = 0;
	other.m_isCompiled = false;

	return *this;
}

void ShaderProgram::setInt(const std::string& name, const gl::GLint value) const {
	GL_CALL(glUniform1i(getUniformLocation(name), value));
}

void ShaderProgram::setFloat(const std::string& name, gl::GLfloat value) const {
	GL_CALL(glUniform1f(getUniformLocation(name), value));
}

void ShaderProgram::setMat4(const std::string& name, const glm::mat4& value) const {
	GL_CALL(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value)));
}

GLint ShaderProgram::getUniformLocation(const std::string& name) const {
	return glGetUniformLocation(m_id, name.c_str());
}

void ShaderProgram::setVec3(const std::string& name, const glm::vec3& value) const {
	glUniform3f(getUniformLocation(name), value.x, value.y, value.z);
}
