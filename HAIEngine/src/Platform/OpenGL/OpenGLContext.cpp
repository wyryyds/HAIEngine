#include "hepch.h"
#include "OpenGLContext.h"
#include "GLFW/glfw3.h"
#include "glad/glad.h"

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
		std::cout<<glGetString(GL_VENDOR)<<" "<< glGetString(GL_RENDERER);
		
		//HE_CORE_INFO("OpenGL Renderer:{0} {1}",glGetString(GL_VENDOR), glGetString(GL_RENDERER));
	}
	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}