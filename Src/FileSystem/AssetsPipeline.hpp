#pragma once
#include "Core/Singleton.h"
#include "Core/Core.hpp"
#include "Renderer/Texture.hpp"

#include <string>
#include <unordered_set>
#include <unordered_map>

namespace HAIEngine::AssetsPipeline
{
	void LoadAllAssets();
	void LoadAllTextures();

	std::shared_ptr<Texture2D> LoadTexture2D(std::string name, std::string path);
	//std::shared_ptr<Texture3D> LoadTexture3D(std::string name, std::string path);

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

		std::shared_ptr<Texture2D> LoadTexture2D(std::string name, std::string path);
		std::shared_ptr<Texture3D> LoadTexture3D(std::string name, std::string path);
		inline std::shared_ptr<Texture2D> GetTexture2D(std::string_view name) { return m_loaded2DTextures[name.data()]; }
		//inline std::shared_ptr<Texture3D> GetTexture3D(std::string_view name) { return m_loaded3DTextures[name.data()]; }
	private:
		std::string m_assetsRootPath = ASSETSPATH;
		std::unordered_set<std::string> m_loadedFiles;
		std::unordered_map<std::string, std::shared_ptr<Texture2D> > m_loaded2DTextures;
		std::unordered_map<std::string, std::shared_ptr<Texture3D> > m_loaded3DTextures;
	};
}