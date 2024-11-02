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
		~Scene() = default;

		void Load();
		void Save();

		inline void AddGameObject(std::shared_ptr<GameObject> go){ m_gameObjects.push_back(go); }
		inline void SetMainCamera(Camera* camera){ m_mainCamera = camera; }

		std::shared_ptr<GameObject> Find(std::string_view name);
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