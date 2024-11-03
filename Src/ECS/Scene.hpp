#pragma once
#include "Core/ISerialize.hpp"

namespace HAIEngine
{
	class Camera;
	class GameObject;

	class Scene : ISerializable
	{
	public:
		Scene(std::string_view filepath);
		Scene(std::string sceneName, std::string_view filepath);
		Scene(const Scene& other) = default;
		Scene& operator=(const Scene& other) = default;
		Scene(Scene&& other) = default;
		Scene& operator=(Scene&& other) = default;
		virtual ~Scene() override = default;

		void Load();
		void Save();

		inline void AddGameObject(const std::shared_ptr<GameObject>& go){ m_gameObjects.push_back(go); }
		inline void SetMainCamera(Camera* camera){ m_mainCamera = camera; }

		std::shared_ptr<GameObject> Find(std::string_view name);
	public:
		std::string m_sceneName;
		Camera* m_mainCamera = nullptr;
		std::vector<std::shared_ptr<GameObject> > m_gameObjects;

	private:
		json Serialize() override;
		void DeSerialize(const json& jsonData) override;
		
	private:
		HESerializeFile m_serializeFile;
	};
}