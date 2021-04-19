#pragma once

namespace MadSword {
	class VertexBuffer {
	public:
		virtual ~VertexBuffer(){}

		virtual void Bind() = 0;
		virtual void UnBind() = 0;

		static VertexBuffer* Create(float* vertices,unsigned int size);
	};

	class IndexBuffer {
	public:
		virtual ~IndexBuffer(){}

		virtual void Bind() = 0;
		virtual void UnBind() = 0;

		virtual unsigned int GetCount() = 0;

		static IndexBuffer* Create(unsigned int* indices, unsigned int size);
	};
}