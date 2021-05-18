#pragma once

#include "MadSword/Renderer/Shader.h"

namespace MadSword {
	class OpenGLShader :public Shader {
	public:
		OpenGLShader(const std::string& vertSrc, const std::string& fragSrc);
		~OpenGLShader() override;

		void Bind() override;
		void UnBind() override;
		void SetMat4(const std::string& name, const mat4& value);
		void SetVec4(const std::string& name, const vec4& value);
		void SetVec3(const std::string& name, const vec3& value);
		void SetVec2(const std::string& name, const vec2& value);
		void SetFloat(const std::string& name, float value);

		void SetVec4i(const std::string& name, const ivec4& value);
		void SetVec3i(const std::string& name, const ivec3& value);
		void SetVec2i(const std::string& name, const ivec2& value);
		void SetInt(const std::string& name, int value);

		void SetMat3(const std::string& name, const mat3& value);
	private:
		unsigned int m_RendererID;
	};
}