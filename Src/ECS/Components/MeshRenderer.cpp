#include "MeshRenderer.hpp"
#include "MeshFilter.hpp"
#include "Platform/OpenGL/OpenGLShader.hpp"
#include <Renderer/FrameBuffer.hpp>
#include <glad/glad.h>
#include <Platform/OpenGL/OpenGLFrameBuffer.hpp>

namespace HAIEngine
{
	void MeshRenderer::Draw(std::shared_ptr<OpenGLShader> shader, std::shared_ptr<FrameBuffer> depthMap)
	{
		auto& meshDatas = m_meshFilter->GetMeshDatas();
		for (unsigned int i = 0; i < meshDatas.size(); ++i)
		{
			auto& meshTextures = meshDatas[i].GetMeshTextures();
			for (unsigned int j = 0; j < meshTextures.size(); ++j)
			{
				unsigned int diffuseNr = 1;
				unsigned int specularNr = 1;
				unsigned int normalNr = 1;
				unsigned int heightNr = 1;

				glActiveTexture(GL_TEXTURE0 + j); // active proper texture unit before binding
				// retrieve texture number (the N in diffuse_textureN)
				std::string number;
				std::string name = meshTextures[j].type;
				if (name == "texture_diffuse")
					number = std::to_string(diffuseNr++);
				else if (name == "texture_specular")
					number = std::to_string(specularNr++); // transfer unsigned int to string
				else if (name == "texture_normal")
					number = std::to_string(normalNr++); // transfer unsigned int to string
				else if (name == "texture_height")
					number = std::to_string(heightNr++); // transfer unsigned int to string

				// now set the sampler to the correct texture unit
				shader->Bind();
				shader->UploadUniformInt((name + number).c_str(), j);
				// and finally bind the texture
				glBindTexture(GL_TEXTURE_2D, meshTextures[j].id);
			}
			// depth map
			shader->Bind();
			glActiveTexture(GL_TEXTURE0 + meshTextures.size());
			std::dynamic_pointer_cast<OpenGLDepthMap>(depthMap)->UseDepthMap(meshTextures.size());
			shader->UploadUniformInt("u_shadowMap", meshTextures.size());
			// draw mesh
			glBindVertexArray(meshDatas[i].VAO);
			glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(meshDatas[i].GetMeshIndices().size()), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
			// always good practice to set everything back to defaults once configured.
			glActiveTexture(GL_TEXTURE0);
		}
	}

	json MeshRenderer::Serialize()
	{
		return json();
	}

	void MeshRenderer::DeSerialize(const json& jsonData)
	{
	}

	void MeshRenderer::GuiDisplay()
	{
	}

}