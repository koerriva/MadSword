#include "mspch.h"

#include "Buffer.h"
#include "Renderer.h"
#include "VertexArray.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace MadSword {
	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::OpenGL:
			return new OpenGLVertexArray();
		default:
			MS_CORE_ASSERT(false, "RendererAPI Not Support Yet!");
			break;
		}
		return nullptr;
	}
}