#pragma once
#include <cstdint>

namespace HAIEngine
{
	enum class ShaderDataType
	{
		None   = 0,
		Float  = 1,
		Float2 = 2,
		Float3 = 3,
		Float4 = 4,
		Mat3   = 5,
		Mat4   = 6,
		Int    = 7,
		Int2   = 8,
		Int3   = 9,
		Int4   = 10,
		Bool   = 11
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case HAIEngine::ShaderDataType::None:   return 0;
			break; 
		case HAIEngine::ShaderDataType::Float:  return 4;
			break;
		case HAIEngine::ShaderDataType::Float2: return 8;
			break;
		case HAIEngine::ShaderDataType::Float3: return 12;
			break;
		case HAIEngine::ShaderDataType::Float4: return 16;
			break;
		case HAIEngine::ShaderDataType::Mat3:   return 36;
			break;
		case HAIEngine::ShaderDataType::Mat4:   return 64;
			break;
		case HAIEngine::ShaderDataType::Int:    return 4;
			break;
		case HAIEngine::ShaderDataType::Int2:   return 8;
			break;
		case HAIEngine::ShaderDataType::Int3:   return 12;
			break;
		case HAIEngine::ShaderDataType::Int4:   return 16;
			break;
		case HAIEngine::ShaderDataType::Bool:   return 4;
			break;
		default: return 0;
			break;
		}
	}

	struct BufferElement
	{
		std::string Name;
		ShaderDataType Type;
		uint32_t Offset;
		uint32_t Size;

		BufferElement(const std::string& name, ShaderDataType type) :Name(name), Type(type), Offset(0), Size(0)
		{

		}
	};

	class BufferLayout
	{
	public:
		inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }
	private:
		std::vector<BufferElement> m_Elements;
	};

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {}

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		static VertexBuffer* Create(float* vertices, uint32_t size);
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {}
		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual uint32_t GetCount() const = 0;

		static IndexBuffer* Create(uint32_t* indices, uint32_t size);
	};
}