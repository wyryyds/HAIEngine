#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
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
		unsigned int id = 0;
		std::string type;
		std::string path;
	};

	class MeshData
	{
	public:
		MeshData(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<MeshTexture>& textures);
		MeshData(const MeshData&) = default;
		MeshData(MeshData&&) = default;
		MeshData& operator=(const MeshData&) = default;
		MeshData& operator=(MeshData&&) = default;
		~MeshData() = default;

		inline const std::vector<Vertex>& GetMeshVertices() const { return m_vertices; }
		inline const std::vector<unsigned int>& GetMeshIndices() const { return m_indices; }
		inline const std::vector<MeshTexture>& GetMeshTextures() const { return m_textures; }

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
		Mesh(const Mesh& other) = default;
		Mesh(Mesh&& other) = default;
		Mesh& operator=(Mesh&&) = default;
		Mesh& operator=(const Mesh&) = default;
		~Mesh() = default;
		
		void LoadMesh(std::string_view filepath);
		void SetMesh();
		void processNode(aiNode* node, const aiScene* scene);
		void processMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<MeshTexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string_view typeName);
		unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma);

		inline std::vector<MeshData>& GetMeshData() { return m_meshDataList; }

	public:
		std::vector<MeshData> m_meshDataList;
		std::vector<MeshTexture> loadedTextures;

	private:
		std::string m_filepath;
		std::string m_directory;
	};


}