#pragma once

#include <MadSword/Renderer/Buffer.h>

namespace MadSword {
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float* vertices, unsigned int size);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void UnBind() const override;
		virtual void SetLayout(const BufferLayout& layout) override;
		virtual const BufferLayout& GetLayout() const override;
	private:
		unsigned int m_RendererID;
		BufferLayout m_Layout;
	};

	class OpenGLIndexBuffer : public IndexBuffer{
	public:
		OpenGLIndexBuffer(unsigned int* indices, unsigned int size);
		virtual ~OpenGLIndexBuffer(){}

		virtual void Bind() const override;
		virtual void UnBind() const override;

		virtual unsigned int GetCount() const { return m_Count; }
	private:
		unsigned int m_RendererID;
		unsigned int m_Count;
	};
}
