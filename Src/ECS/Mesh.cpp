#include "Mesh.h"

#include "Platform/OpenGL/OpenGLShader.hpp"
#include "Platform/OpenGL/OpenGLTexture.hpp"

#include <glad/glad.h>
#include <iostream>
#include <stb_image.h>

namespace HAIEngine
{
	MeshData::MeshData(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<MeshTexture>& textures)
		: VAO(0), VBO(0), EBO(0)
	{
		m_vertices = vertices;
		m_indices = indices;
		m_textures = textures;
	}

	Mesh::Mesh(std::string_view filepath)
	{
		LoadMesh(filepath);
		SetMesh();
	}

	void Mesh::SetMesh()
	{
		for (int i = 0; i < m_meshDatas.size(); ++i)
		{
			glGenVertexArrays(1, &m_meshDatas[i].VAO);
			glGenBuffers(1, &m_meshDatas[i].VBO);
			glGenBuffers(1, &m_meshDatas[i].EBO);

			glBindVertexArray(m_meshDatas[i].VAO);
			// load data into vertex buffers
			glBindBuffer(GL_ARRAY_BUFFER, m_meshDatas[i].VBO);
			// A great thing about structs is that their memory layout is sequential for all its items.
			// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
			// again translates to 3/2 floats which translates to a byte array.
			glBufferData(GL_ARRAY_BUFFER, m_meshDatas[i].m_vertices.size() * sizeof(Vertex), &m_meshDatas[i].m_vertices[0], GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_meshDatas[i].EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_meshDatas[i].m_indices.size() * sizeof(unsigned int), &m_meshDatas[i].m_indices[0], GL_STATIC_DRAW);

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

	void Mesh::LoadMesh(std::string_view filepath)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(filepath.data(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
		// check for errors
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
		{
			std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
			return;
		}
		// retrieve the directory path of the filepath
		directory = filepath.substr(0, filepath.find_last_of('/'));

		// process ASSIMP's root node recursively
		processNode(scene->mRootNode, scene);
	}
	void Mesh::processNode(aiNode* node, const aiScene* scene)
	{
		// 处理节点所有的网格（如果有的话）
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			processMesh(mesh, scene);
		}
		// 接下来对它的子节点重复这一过程
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}
	}

	void Mesh::processMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<MeshTexture> textures;

		// walk through each of the mesh's vertices
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex{};
			glm::vec3 vector{}; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
			// positions
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.position = vector;
			// normals
			if (mesh->HasNormals())
			{
				vector.x = mesh->mNormals[i].x;
				vector.y = mesh->mNormals[i].y;
				vector.z = mesh->mNormals[i].z;
				vertex.normal = vector;
			}
			// texture coordinates
			if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
			{
				glm::vec2 vec{};
				// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
				// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.texCoords = vec;
				if (mesh->HasTangentsAndBitangents())
				{
					//tangent
					vector.x = mesh->mTangents[i].x;
					vector.y = mesh->mTangents[i].y;
					vector.z = mesh->mTangents[i].z;
					vertex.tangent = vector;
					// bitangent
					vector.x = mesh->mBitangents[i].x;
					vector.y = mesh->mBitangents[i].y;
					vector.z = mesh->mBitangents[i].z;
					vertex.biTangent = vector;
				}
			}
			else
				vertex.texCoords = glm::vec2(0.0f, 0.0f);

			vertices.push_back(vertex);
		}
		// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			// retrieve all indices of the face and store them in the indices vector
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}
		// process materials
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		// we assume a convention for sampler names in the shaders. Each diffuse texture should be named
		// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
		// Same applies to other texture as the following list summarizes:
		// diffuse: texture_diffuseN
		// specular: texture_specularN
		// normal: texture_normalN

		// 1. diffuse maps
		std::vector<MeshTexture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		// 2. specular maps
		std::vector<MeshTexture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		// 3. normal maps
		std::vector<MeshTexture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		// 4. height maps
		std::vector<MeshTexture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
		// push into 
		m_meshDatas.push_back({ vertices, indices, textures });
	}

	std::vector<MeshTexture> Mesh::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
	{
		std::vector<MeshTexture> textures;
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
			bool skip = false;
			for (unsigned int j = 0; j < textures_loaded.size(); j++)
			{
				if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
				{
					textures.push_back(textures_loaded[j]);
					skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
					break;
				}
			}
			if (!skip)
			{   // if texture hasn't been loaded already, load it
				MeshTexture texture;
				texture.id = TextureFromFile(str.C_Str(), this->directory, false);
				texture.type = typeName;
				texture.path = str.C_Str();
				textures.push_back(texture);
				textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecessary load duplicate textures.
			}
		}
		return textures;
	}

	unsigned int Mesh::TextureFromFile(const char* path, const std::string& directory, bool gamma)
	{
		std::string filename = std::string(path);
		filename = directory + '/' + filename;

		unsigned int textureID;
		glGenTextures(1, &textureID);
		stbi_set_flip_vertically_on_load(0);
		int width, height, nrComponents;
		unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
		if (data)
		{
			GLenum format{};
			if (nrComponents == 1)
				format = GL_RED;
			else if (nrComponents == 3)
				format = GL_RGB;
			else if (nrComponents == 4)
				format = GL_RGBA;

			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(data);
		}
		else
		{
			std::cout << "Texture failed to load at path: " << path << std::endl;
			stbi_image_free(data);
		}

		return textureID;
	}
}