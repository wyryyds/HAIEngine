#include "hepch.h"

#include "RenderCommand.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"
namespace HAIEngine
{
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI();
}