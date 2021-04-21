#include "mspch.h"

#include "Buffer.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace MadSword{
	VertexBuffer* MadSword::VertexBuffer::Create(float* vertices, unsigned int size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::OpenGL:
			return new OpenGLVertexBuffer(vertices, size);
		default:
			MS_CORE_ASSERT(false, "RendererAPI Not Support Yet!");
			break;
		}
		return nullptr;
	}

	IndexBuffer* MadSword::IndexBuffer::Create(unsigned int* indices, unsigned int size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::OpenGL:
			return new OpenGLIndexBuffer(indices, size);
		default:
			MS_CORE_ASSERT(false, "RendererAPI Not Support Yet!");
			break;
		}
		return nullptr;
	}
}