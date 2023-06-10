#include"hepch.h"
#include"HAIEngine/Renderer/VertexArray.h"
#include"Renderer.h"
#include"Platform/OpenGL/OpenGLVertexArray.h"

namespace HAIEngine
{
	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			HE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return new OpenGLVertexArray();
		}
		HE_CORE_ASSERT(false, "UnKnown RendererAPI!");
		return nullptr;
	}
}