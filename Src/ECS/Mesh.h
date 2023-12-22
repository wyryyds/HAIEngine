#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <string>
#include <vector>

namespace HAIEngine
{
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texCoords;
		glm::vec3 tangent;
		glm::vec3 biTangent;
	};

	struct MeshTexture
	{
		unsigned int id;
		std::string type;
		std::string path;
	};

	class MeshData
	{
	public:
		MeshData(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<MeshTexture>& textures);
		~MeshData() = default;

	public:
		std::vector<Vertex> m_vertices;
		std::vector<unsigned int> m_indices;
		std::vector<MeshTexture> m_textures;

		unsigned int VAO, VBO, EBO;
	};

	class Mesh
	{
	public:
		Mesh(std::string_view filepath);
		~Mesh() = default;
		
		void LoadMesh(std::string_view filepath);
		void SetMesh();
		void processNode(aiNode* node, const aiScene* scene);
		void processMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<MeshTexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
		unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma);

	public:
		std::vector<MeshData> m_meshDatas;
		std::vector<MeshTexture> textures_loaded;

	private:
		std::string m_filepath;
		std::string directory;
	};


}