#pragma once

#include <MadSword/Renderer/Buffer.h>

namespace MadSword {
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float* vertices, unsigned int size);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() override;
		virtual void UnBind() override;
	private:
		unsigned int m_RendererID;
	};

	class OpenGLIndexBuffer : public IndexBuffer{
	public:
		OpenGLIndexBuffer(unsigned int* indices, unsigned int size);
		virtual ~OpenGLIndexBuffer(){}

		virtual void Bind() override;
		virtual void UnBind() override;

		virtual unsigned int GetCount() { return m_Count; }
	private:
		unsigned int m_RendererID;
		unsigned int m_Count;
	};
}
