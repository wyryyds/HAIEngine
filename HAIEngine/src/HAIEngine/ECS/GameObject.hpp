#pragma once
#include "Core/ISerialize.hpp"
#include "Component.hpp"
#include "Components/Transform.hpp"
#include "Core/Log.hpp"

namespace HAIEngine
{
	class Transform;

	class GameObject : public ISerializeable
	{
	public:
		GameObject();
		GameObject(std::string name);
		GameObject(std::string name, size_t guid);
		virtual ~GameObject();

		void Update();

		virtual json Serialize(const std::string& name) override;
		virtual void DeSerialize(const json& data) override;

		void AddComponent(Component* component);

		template <class T>
		T* GetComponent()
		{
			for (auto iter = m_components.begin(); iter != m_components.end(); ++iter)
			{
				auto com = dynamic_cast<T*>(*iter);
				if (com)
				{
					return *com;
				}
			}
			LOG_Error("No such component!");
			return nullptr;
		}

		template <class T>
		bool RemoveComponent()
		{
			for (auto iter = m_components.begin(); iter != m_components.end();)
			{
				auto com = dynamic_cast<T*>(*iter);
				if (com)
				{
					m_components.erase(iter);
					return true;
				}
				else
				{
					++iter;
				}
			}
			LOG_Error("No such component at this gameobejct!");
			return false;
		}

		template <class T>
		bool HasComponent()
		{
			for (auto iter = m_components.begin(); iter != m_components.end(); ++iter)
			{
				auto com = dynamic_cast<T*>(*iter);
				if (com)
					return true;
			}
			LOG_Error("No such component at this gameobejct!");
			return false;
		}

		static size_t GenerateGUID(const GameObject* obj) {
			return std::hash<const GameObject*>{}(obj);
		}

		size_t GetGUID() const {
			return m_guid;
		}
	public:
		std::string m_name;
		size_t m_guid;

		Transform* m_transform;
	private:
		std::unordered_map<std::string, Component*> m_components;
	};
}