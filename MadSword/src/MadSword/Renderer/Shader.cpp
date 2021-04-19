#include "mspch.h"
#include "Shader.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace MadSword {
	Shader* Shader::Create(const std::string& vertSrc, const std::string& fragSrc)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::OpenGL:return new OpenGLShader(vertSrc,fragSrc);
		default:
			break;
		}
		return nullptr;
	}
}