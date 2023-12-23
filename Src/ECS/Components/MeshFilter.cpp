#include "MeshFilter.hpp"
#include "Platform/OpenGL/OpenGLShader.hpp"
#include "Platform/OpenGL/OpenGLTexture.hpp"

#include <glad/glad.h>
#include <iostream>
#include <stb_image.h>

namespace HAIEngine
{

	json MeshFilter::Serialize()
	{
		return json();
	}

	void MeshFilter::DeSerialize(const json& jsonData)
	{
	}

	void MeshFilter::GuiDisplay()
	{
	}

}