#pragma once

namespace MadSword {
	enum class ShaderDataType {
		None=0,Float,Vec2f,Vec3f,Vec4f,Mat3,Mat4,Int,Vec2i,Vec3i,Vec4i,Bool
	};
	static unsigned int ShaderDataTypeSize(ShaderDataType type) {
		unsigned int size = 0;
		switch (type)
		{
		case MadSword::ShaderDataType::None:
			MS_CORE_ASSERT(false, "Unknown ShaderDataType::None");
			break;
		case MadSword::ShaderDataType::Float:
			size = 4;
			break;
		case MadSword::ShaderDataType::Vec2f:
			size = 4 * 2;
			break;
		case MadSword::ShaderDataType::Vec3f:
			size = 4 * 3;
			break;
		case MadSword::ShaderDataType::Vec4f:
			size = 4 * 4;
			break;
		case MadSword::ShaderDataType::Mat3:
			size = 4 * 3 * 3;
			break;
		case MadSword::ShaderDataType::Mat4:
			size = 4 * 4 * 4;
			break;
		case MadSword::ShaderDataType::Int:
			size = 4;
			break;
		case MadSword::ShaderDataType::Vec2i:
			size = 4 * 2;
			break;
		case MadSword::ShaderDataType::Vec3i:
			size = 4 * 3;
			break;
		case MadSword::ShaderDataType::Vec4i:
			size = 4 * 4;
			break;
		case MadSword::ShaderDataType::Bool:
			size = 1;
			break;
		default:
			MS_CORE_ASSERT(false, "Unknown ShaderDataType::Default");
			break;
		}
		return size;
	}
	struct BufferElement {
		std::string Name;
		ShaderDataType Type;
		unsigned int Size;
		unsigned int Offset;
		bool Normalized;

		BufferElement() {}
		BufferElement(ShaderDataType type, const std::string& name, bool normalized=false) :
			Name(name), Type(type) , Size(ShaderDataTypeSize(type)), Offset(0), Normalized(false)
		{}

		unsigned int GetComponentCount() {
			unsigned int count = 0;
			switch (Type)
			{
			case MadSword::ShaderDataType::None:
				MS_CORE_ASSERT(false, "Unknown ShaderDataType::None");
				break;
			case MadSword::ShaderDataType::Float:
				count = 1;
				break;
			case MadSword::ShaderDataType::Vec2f:
				count = 2;
				break;
			case MadSword::ShaderDataType::Vec3f:
				count = 3;
				break;
			case MadSword::ShaderDataType::Vec4f:
				count = 4;
				break;
			case MadSword::ShaderDataType::Mat3:
				count = 3 * 3;
				break;
			case MadSword::ShaderDataType::Mat4:
				count = 4 * 4;
				break;
			case MadSword::ShaderDataType::Int:
				count = 1;
				break;
			case MadSword::ShaderDataType::Vec2i:
				count = 2;
				break;
			case MadSword::ShaderDataType::Vec3i:
				count = 3;
				break;
			case MadSword::ShaderDataType::Vec4i:
				count = 4;
				break;
			case MadSword::ShaderDataType::Bool:
				count = 1;
				break;
			default:
				MS_CORE_ASSERT(false, "Unknown ShaderDataType::Default");
				break;
			}
			return count;
		}
	};
	class BufferLayout {
	public:
		BufferLayout() {}
		BufferLayout(const std::initializer_list<BufferElement>& elements) :m_Elements(elements) 
		{
			CalcOffsetAndStride();
		}

		inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }

		inline unsigned int GetStride() const { return m_Stride; }

		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
	private:
		void CalcOffsetAndStride() {
			unsigned int offset = 0;
			m_Stride = 0;
			for (auto& element : m_Elements)
			{
				element.Offset = offset;
				offset += element.Size;
				m_Stride += element.Size;
			}
		}
	private:
		std::vector<BufferElement> m_Elements;
		unsigned int m_Stride = 0;
	};
	class VertexBuffer {
	public:
		virtual ~VertexBuffer(){}

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;
		virtual const BufferLayout& GetLayout() const = 0;

		static VertexBuffer* Create(float* vertices,unsigned int size);
	};

	class IndexBuffer {
	public:
		virtual ~IndexBuffer(){}

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual unsigned int GetCount() const = 0;

		static IndexBuffer* Create(unsigned int* indices, unsigned int size);
	};
}