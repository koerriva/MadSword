#pragma once

#include "MadSword/Renderer/Shader.h"

namespace MadSword {
	class OpenGLShader :public Shader {
	public:
		OpenGLShader(const std::string& vertSrc, const std::string& fragSrc);
		virtual ~OpenGLShader();

		virtual void Bind() override;
		virtual void UnBind() override;
	private:
		unsigned int m_RendererID;
	};
}