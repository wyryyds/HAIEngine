#include "AssetsPipeline.hpp"

#include <filesystem>

namespace HAIEngine::AssetsPipeline
{
	void LoadAllAssets()
	{
		for (const auto& entry : std::filesystem::recursive_directory_iterator(FileSystem::GetInstance().GetDirectoryPath()))
		{
			const auto& path = entry.path();
			// if texture file
			if (entry.is_regular_file() && (path.extension() == ".png" || path.extension() == ".jpg" || path.extension() == ".jpeg"))
			{
				LoadTexture(path.stem().string(), path.string());
			}
			else if (entry.is_regular_file() && (path.extension() == ".glsl")) // if shader file
			{
				LoadShader(path.stem().string(), path.string());
			}
		}
	}

	bool LoadAllTextures()
	{
		for (const auto& entry : std::filesystem::recursive_directory_iterator(FileSystem::GetInstance().GetDirectoryPath())) 
		{
			const auto& path = entry.path();
			if (entry.is_regular_file() && (path.extension() == ".png" || path.extension() == ".jpg" || path.extension() == ".jpeg")) 
			{
				LoadTexture(path.stem().string(), path.string());
			}
		}
		return true;
	}

	bool LoadAllShaders()
	{
		for (const auto& entry : std::filesystem::recursive_directory_iterator(FileSystem::GetInstance().GetDirectoryPath()))
		{
			const auto& path = entry.path();
			if (entry.is_regular_file() && (path.extension() == ".glsl"))
			{
				LoadShader(path.stem().string(), path.string());
			}
		}
		return true;
	}

	std::shared_ptr<Texture2D> LoadTexture(std::string name, std::string path)
	{
		return FileSystem::GetInstance().LoadTexture(name, path);
	}

	std::shared_ptr<Texture2D> GetTexture(std::string_view name)
	{
		return FileSystem::GetInstance().GetTexture(name);
	}

	std::shared_ptr<Shader> LoadShader(std::string name, std::string path)
	{
		return FileSystem::GetInstance().LoadShader(name, path);
	}

	std::shared_ptr<Shader> GetShader(std::string_view name)
	{
		return FileSystem::GetInstance().GetShader(name);
	}

	// fileSystem
	std::shared_ptr<Texture2D> FileSystem::LoadTexture(std::string name, std::string path)
	{
		if (IsFileLoaded(path)) return FileSystem::GetInstance().GetTexture(name);
		auto texture = Texture2D::Create(path);
		m_loadedTextures.insert({ name, texture });
		return texture;
	}

	std::shared_ptr<Texture2D> FileSystem::GetTexture(std::string_view name)
	{
		return m_loadedTextures[name.data()];
	}

	std::shared_ptr<Shader> FileSystem::LoadShader(std::string name, std::string path)
	{
		return m_shaderLibrary.Load(name, path);
	}

	std::shared_ptr<Shader> FileSystem::GetShader(std::string_view name)
	{
		return m_shaderLibrary.Get(name);
	}

}