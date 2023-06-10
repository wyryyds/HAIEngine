#pragma once

#include"RenderCommand.h"

namespace HAIEngine
{

	class Renderer
	{
	public:
		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
	};
}