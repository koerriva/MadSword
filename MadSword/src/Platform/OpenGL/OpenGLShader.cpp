#include "mspch.h"
#include "OpenGLShader.h"
#include <glad/glad.h>

#include "glm/gtc/type_ptr.hpp"

namespace MadSword {
	OpenGLShader::OpenGLShader(const std::string& vertSrc, const std::string& fragSrc)
	{
		// Create an empty vertex shader handle
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// Send the vertex shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		const GLchar* source = (const GLchar*)vertSrc.c_str();
		glShaderSource(vertexShader, 1, &source, 0);

		// Compile the vertex shader
		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(vertexShader);

			// Use the infoLog as you see fit.
			MS_CORE_ERROR("{}", infoLog.data());
			MS_CORE_ASSERT(false, "VERTEX::COMPILE::ERROR");
		}

		// Create an empty fragment shader handle
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Send the fragment shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		source = (const GLchar*)fragSrc.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);

		// Compile the fragment shader
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(fragmentShader);
			// Either of them. Don't leak shaders.
			glDeleteShader(vertexShader);

			// Use the infoLog as you see fit.
			MS_CORE_ERROR("{}", infoLog.data());
			MS_CORE_ASSERT(false, "FRAGMENT::COMPILE::ERROR");
		}

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		GLuint program = glCreateProgram();

		// Attach our shaders to our program
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);

		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);
			// Don't leak shaders either.
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			// Use the infoLog as you see fit.
			MS_CORE_ERROR("{}", infoLog.data());
			MS_CORE_ASSERT(false, "SHADER::COMPILE::ERROR");
		}

		// Always detach shaders after a successful link.
		glDetachShader(program, vertexShader);
		glDetachShader(program, fragmentShader);

		m_RendererID = program;
	}
	OpenGLShader::~OpenGLShader()
	{
		MS_CORE_TRACE("Unload OpenGLShader:{}", m_RendererID);
		glDeleteProgram(m_RendererID);
	}
	void OpenGLShader::Bind()
	{
		glUseProgram(m_RendererID);
	}
	void OpenGLShader::UnBind()
	{
		glUseProgram(0);
	}
	void OpenGLShader::SetMat4(const std::string& name, const mat4& value)
	{
		const unsigned int location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE,value_ptr(value));
	}
	void OpenGLShader::SetVec4(const std::string& name, const vec4& value)
	{
		const unsigned int location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}
	void OpenGLShader::SetVec3(const std::string& name, const vec3& value)
	{
		const unsigned int location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	}
	void OpenGLShader::SetVec2(const std::string& name, const vec2& value)
	{
		const unsigned int location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2f(location, value.x, value.y);
	}
	void OpenGLShader::SetFloat(const std::string& name, float value)
	{
		const unsigned int location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(location, value);
	}
	void OpenGLShader::SetVec4i(const std::string& name, const ivec4& value)
	{
		const unsigned int location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4i(location, value.x, value.y, value.z, value.w);
	}
	void OpenGLShader::SetVec3i(const std::string& name, const ivec3& value)
	{
		const unsigned int location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3i(location, value.x, value.y, value.z);
	}
	void OpenGLShader::SetVec2i(const std::string& name, const ivec2& value)
	{
		const unsigned int location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2i(location, value.x, value.y);
	}
	void OpenGLShader::SetInt(const std::string& name, int value)
	{
		const unsigned int location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, value);
	}
	void OpenGLShader::SetMat3(const std::string& name, const mat3& value)
	{
		const unsigned int location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, value_ptr(value));
	}
}
