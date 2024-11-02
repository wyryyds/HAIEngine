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

	void MeshFilter::BindMeshData() const
	{
		auto& meshData = m_mesh->GetMeshData();
		for (int i = 0; i < meshData.size(); ++i)
		{
			glGenVertexArrays(1, &meshData[i].VAO);
			glGenBuffers(1, &meshData[i].VBO);
			glGenBuffers(1, &meshData[i].EBO);

			glBindVertexArray(meshData[i].VAO);
			// load data into vertex buffers
			glBindBuffer(GL_ARRAY_BUFFER, meshData[i].VBO);
			// A great thing about structs is that their memory layout is sequential for all its items.
			// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
			// again translates to 3/2 floats which translates to a byte array.
			glBufferData(GL_ARRAY_BUFFER, meshData[i].m_vertices.size() * sizeof(Vertex), &meshData[i].m_vertices[0], GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshData[i].EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshData[i].m_indices.size() * sizeof(unsigned int), &meshData[i].m_indices[0], GL_STATIC_DRAW);

			// set the vertex attribute pointers
			// vertex Positions
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
			// vertex normals
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
			// vertex texture coords
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
			// vertex tangent
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
			// vertex bitangent
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, biTangent));

			glBindVertexArray(0);
		}
	}

}