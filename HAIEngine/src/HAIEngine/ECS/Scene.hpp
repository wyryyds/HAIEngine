#pragma once
#include "HAIEngine/ECS/GameObject.hpp"
#include "HAIEngine/ECS/Components/Camera.hpp"

namespace HAIEngine
{
	class Scene : ISerializeable
	{
	public:
		Scene(const std::string& filepath);
		Scene(std::string sceneName, const std::string& filepath);
		~Scene() = default;

		void Save();

		inline void AddGameObject(std::shared_ptr<GameObject> go)
		{
			m_gameObjects.push_back(go);
		}

		std::shared_ptr<GameObject> Find(const std::string& name);
	public:
		std::string m_sceneName;
		Camera* m_mainCamera;
		std::vector<std::shared_ptr<GameObject> > m_gameObjects;

	private:
		json Serialize(const std::string& name) override;
		void DeSerialize(const json& jsondata) override;
	private:
		HESerializeFile m_serializeFile;
	};
}