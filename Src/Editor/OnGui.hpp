#pragma once
#include <string>
#include <glm/ext/vector_float3.hpp>

namespace HAIEngine
{
	class IGuiDisplay
	{
	public:
		IGuiDisplay() = default;
		virtual ~IGuiDisplay() = default;
		
		virtual void GuiDisplay() = 0;
	};
}