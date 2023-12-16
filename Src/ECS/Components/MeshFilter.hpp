#pragma once
#include "Ecs/Component.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace HAIEngine
{
	struct Vertex
	{
		// position
		glm::vec3 Position;
		// normal
		glm::vec3 Normal;
		// texCoords
		glm::vec2 TexCoords;
		// tangent
		glm::vec3 Tangent;
		// bitangent
		glm::vec3 Bitangent;
	};

	struct TTexture
	{
		unsigned int id;
		std::string type;
		std::string path;
	};

	class OpenGLShader;

	class MeshFilter : public Component
	{
	public:
		MeshFilter() : Component("MeshFilter") {}
		MeshFilter(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices,const std::vector<TTexture>& textures);
		void Draw(std::shared_ptr<OpenGLShader> shader);
	public:
		std::vector<Vertex> m_vertices;
		std::vector<unsigned int> m_indices;
		std::vector<TTexture> m_textures;

		virtual json Serialize() override;
		virtual void DeSerialize(const json& jsonData) override;
		virtual void GuiDisplay() override;
	private:
		void SetMesh();
	private:
		unsigned int VAO, VBO, EBO;

	};

	class Model
	{
	public:
		/*  函数   */
		Model() {}
		Model(char* path)
		{
			loadModel(path);
		}
		void Draw(std::shared_ptr<OpenGLShader> shader);
		std::vector<TTexture> textures_loaded;
	private:
		/*  模型数据  */
		std::vector<MeshFilter> meshes;
		std::string directory;
		/*  函数   */
		void loadModel(std::string const  &path);
		void processNode(aiNode* node, const aiScene* scene);
		MeshFilter processMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<TTexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
		unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma);
	};
}