#pragma once
#include "Core/Singleton.h"
#include "Core/Core.hpp"
#include "Renderer/Texture.hpp"
#include "Renderer/Shader.hpp"

#include <string>
#include <unordered_set>
#include <unordered_map>

namespace HAIEngine::AssetsPipeline
{
	void LoadAllAssets();
	void Test();
	bool LoadAllTextures();
	bool LoadAllShaders();
	std::shared_ptr<Texture2D> LoadTexture(std::string name, std::string path);
	std::shared_ptr<Texture2D> GetTexture(std::string_view name);

	std::shared_ptr<Shader> LoadShader(std::string name, std::string path);
	std::shared_ptr<Shader> GetShader(std::string_view name);
	
	enum class AssetType
	{
		Unknow  = 0,
		Script  = 1,
		Texture = 2,
		Shader  = 3,

	};

	class FileSystem : public Singleton<FileSystem>
	{
	public:
		inline bool IsFileLoaded(std::string_view path) { return m_loadedFiles.find(path.data()) != m_loadedFiles.end(); }
		inline std::string_view GetDirectoryPath() { return m_assetsRootPath; }

		std::shared_ptr<Texture2D> LoadTexture(std::string name, std::string path);
		std::shared_ptr<Texture2D> GetTexture(std::string_view name);

		std::shared_ptr<Shader> LoadShader(std::string name, std::string path);
		std::shared_ptr<Shader> GetShader(std::string_view name);

	private:
		std::string m_assetsRootPath = ASSETSPATH;
		std::unordered_set<std::string> m_loadedFiles;
		std::unordered_map<std::string, std::shared_ptr<Texture2D> > m_loadedTextures;
		ShaderLibrary m_shaderLibrary;
	};
}