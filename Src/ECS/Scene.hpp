#pragma once
#include "Core/ISerialize.hpp"

namespace HAIEngine
{
	class Camera;
	class GameObject;

	class Scene : ISerializeable
	{
	public:
		Scene(const std::string& filepath);
		Scene(std::string sceneName, const std::string& filepath);
		~Scene() = default;

		void Load();
		void Save();

		inline void AddGameObject(std::shared_ptr<GameObject> go){ m_gameObjects.push_back(go); }
		inline void SetMainCamera(Camera* camera){ m_mainCamera = camera; }

		std::shared_ptr<GameObject> Find(const std::string& name);
	public:
		std::string m_sceneName;
		Camera* m_mainCamera;
		std::vector<std::shared_ptr<GameObject> > m_gameObjects;

	private:
		json Serialize() override;
		void DeSerialize(const json& jsondata) override;
	private:
		HESerializeFile m_serializeFile;
	};
}