#include "AssetsPipeline.hpp"

#include <filesystem>

namespace HAIEngine::AssetsPipeline
{
	void LoadAllAssets()
	{
		// TODO;
		//FileSystem::GetInstance();
	}

	void LoadAllTextures()
	{
		// 遍历目录
		for (const auto& entry : std::filesystem::recursive_directory_iterator(FileSystem::GetInstance().GetDirectoryPath())) {
			const auto& path = entry.path();
			// 检查文件类型是否为常见的图像格式
			if (entry.is_regular_file() &&
				(path.extension() == ".png" ||
					path.extension() == ".jpg" ||
					path.extension() == ".jpeg")) {
				// 调用你的LoadTexture函数加载图像
				FileSystem::GetInstance().LoadTexture2D(path.stem().string(), path.string());
			}
		}
	}

	std::shared_ptr<Texture2D> LoadTexture2D(std::string name, std::string path)
	{
		return FileSystem::GetInstance().LoadTexture2D(name, path);
	}

	//std::shared_ptr<Texture3D> LoadTexture3D(std::string name, std::string path)
	//{
	//	//return FileSystem::GetInstance().LoadTexture3D(name, path);
	//}

	std::shared_ptr<Texture2D> FileSystem::LoadTexture2D(std::string name, std::string path)
	{
		if (IsFileLoaded(path)) return FileSystem::GetInstance().GetTexture2D(name);
		auto texture = Texture2D::Create(path);
		m_loaded2DTextures.insert({ name, texture });
		return texture;
	}

	/*std::shared_ptr<Texture3D> FileSystem::LoadTexture3D(std::string name, std::string path)
	{
		
	}*/
}