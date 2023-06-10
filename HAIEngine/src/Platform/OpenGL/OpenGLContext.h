#pragma once
#include "HAIEngine/Renderer/GraphicsContext.h"
#include "HAIEngine/Core.h"

struct GLFWwindow;

namespace HAIEngine
{
	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;
	private:
		GLFWwindow* m_WindowHandle;
	};
}