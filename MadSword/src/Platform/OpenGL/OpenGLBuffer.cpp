#include "mspch.h"
#include "OpenGLBuffer.h"
#include <glad/glad.h>

namespace MadSword {
	static GLenum GetOpenGLShaderDataType(ShaderDataType type) {
		GLenum i = 0;
		switch (type)
		{
		case MadSword::ShaderDataType::None:
			MS_CORE_ASSERT(false, "Unknown ShaderDataType::None");
			break;
		case MadSword::ShaderDataType::Float:
		case MadSword::ShaderDataType::Vec2f:
		case MadSword::ShaderDataType::Vec3f:
		case MadSword::ShaderDataType::Vec4f:
		case MadSword::ShaderDataType::Mat3:
		case MadSword::ShaderDataType::Mat4:
			i = GL_FLOAT;
			break;
		case MadSword::ShaderDataType::Int:
		case MadSword::ShaderDataType::Vec2i:
		case MadSword::ShaderDataType::Vec3i:
		case MadSword::ShaderDataType::Vec4i:
			i = GL_INT;
			break;
		case MadSword::ShaderDataType::Bool:
			i = GL_BOOL;
			break;
		default:
			MS_CORE_ASSERT(false, "Unknown ShaderDataType::Default");
			break;
		}
		return i;
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, unsigned int size)
	{
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}
	OpenGLVertexBuffer::~OpenGLVertexBuffer() {
		glDeleteBuffers(1,&m_RendererID);
	}

	void OpenGLVertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGLVertexBuffer::UnBind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGLVertexBuffer::SetLayout(const BufferLayout& layout)
	{
		m_Layout = layout;

		unsigned int index = 0;
		for (auto& element : m_Layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, element.GetComponentCount(), 
				GetOpenGLShaderDataType(element.Type), 
				element.Normalized?GL_TRUE:GL_FALSE, 
				layout.GetStride(), (const void*)element.Offset);
			index++;
		}
	}

	const BufferLayout& OpenGLVertexBuffer::GetLayout() const
	{
		return m_Layout;
	}

	OpenGLIndexBuffer::OpenGLIndexBuffer(unsigned int* indices, unsigned int size)
	{
		m_Count = size / sizeof(unsigned int);
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
	}
	void OpenGLIndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}
	void OpenGLIndexBuffer::UnBind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}
}