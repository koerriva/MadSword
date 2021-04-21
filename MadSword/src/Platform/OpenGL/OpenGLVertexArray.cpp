#include "mspch.h"
#include "OpenGLVertexArray.h"
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

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_RendererID);
	}
	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}
	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::UnBind() const
	{
		glBindVertexArray(0);
	}
	void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		bool isLayoutEmpty = vertexBuffer->GetLayout().GetElements().size();
		MS_CORE_ASSERT(isLayoutEmpty, "Vertex Buffer Layout is Empty!");

		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		unsigned int index = 0;
		const BufferLayout& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, element.GetComponentCount(),
				GetOpenGLShaderDataType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(), (const void*)element.Offset);
			index++;
		}

		m_VertexBuffers.push_back(vertexBuffer);
	}
	void OpenGLVertexArray::AddIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffers.push_back(indexBuffer);
	}
}
