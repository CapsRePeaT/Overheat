#pragma once

#include <string>
#include <filesystem>

#include <glbinding/gl/enum.h>
#include <glbinding/gl/types.h>
#include <opengllog.h>
#include <glm/gtc/matrix_transform.hpp>

class ShaderProgram
{
public:
	using Path = std::filesystem::path;
	ShaderProgram(const Path& vertexShader, const Path& fragmentShader);
	ShaderProgram(ShaderProgram&& other) noexcept ;
	ShaderProgram& operator=(ShaderProgram&& other) noexcept ;

	ShaderProgram() = delete;
	ShaderProgram(const ShaderProgram&) = delete;
	ShaderProgram& operator=(const ShaderProgram&) = delete;

	~ShaderProgram();

	[[nodiscard]] bool isCompiled() const noexcept { return m_isCompiled; }
	inline void use() const {
		if (m_id != usingId) {
			glCall<gl::glUseProgram>(m_id);
			usingId = m_id;
		}
	}

	[[nodiscard]] gl::GLint getUniformLocation(const std::string& name) const;

	void setInt(const std::string& name, gl::GLint value) const;
	void setFloat(const std::string& name, gl::GLfloat value) const;
	void setMat4(const std::string& name, const glm::mat4& value) const;
	void setVec3(const std::string& name, const glm::vec3& value) const;

private:
	bool m_isCompiled = false;
	gl::GLuint m_id = 0;

	static gl::GLuint usingId;

	static bool _createShader(const std::string& source, gl::GLenum shaderType, gl::GLuint& shaderId);
};
