#include "hepch.hpp"

#include "RenderCommand.hpp"
#include "Platform/OpenGL/OpenGLRendererAPI.hpp"
namespace HAIEngine
{
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI();
}