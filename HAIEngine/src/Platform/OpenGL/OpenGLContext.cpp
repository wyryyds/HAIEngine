#include"hepch.h"
#include"OpenGLContext.h"
#include"GLFW/glfw3.h"
#include"glad/glad.h"

#include "HAIEngine/Log.h"

namespace HAIEngine
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle):m_WindowHandle(windowHandle)
	{
		HE_CORE_ASSERT(windowHandle, "windowHandle is null");
	}
	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		HE_CORE_ASSERT(status, "Failed to initialize Glad!");
	}
	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}