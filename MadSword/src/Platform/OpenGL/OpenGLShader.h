#pragma once

#include "MadSword/Renderer/Shader.h"

namespace MadSword {
	class OpenGLShader :public Shader {
	public:
		OpenGLShader(const std::string& vertSrc, const std::string& fragSrc);
		~OpenGLShader() override;

		void Bind() override;
		void UnBind() override;
		void SetMat4(const std::string& name, const mat4& value) override;
	private:
		unsigned int m_RendererID;
	};
}