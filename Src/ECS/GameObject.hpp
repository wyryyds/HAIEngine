#pragma once
#include "Core/ISerialize.hpp"
#include "Component.hpp"
#include "Core/Log.hpp"

namespace HAIEngine
{
	class Transform;
	class TimeStep;

	class GameObject : public ISerializeable
	{
	public:
		GameObject();
		GameObject(std::string name);
		GameObject(std::string name, size_t guid);
		~GameObject() override;

		void Update(TimeStep ts);

		virtual json Serialize() override;
		virtual void DeSerialize(const json& data) override;

		inline Transform* GetTransform() const { return m_transform; }
		void AddComponent(Component* component);

		template <class T>
		T& GetComponent()
		{
			static_assert(std::is_base_of<Component, T>::value, "T must be derived from Component");

			for (const auto& pair : m_components)
			{
				auto com = dynamic_cast<T*>(pair.second);
				if (com)
				{
					return *com;
				}
			}

			LOG_Error("No such component at this gameobject!");
			static T invalidComponent;
			return invalidComponent;
		}

		template <class T>
		bool RemoveComponent()
		{
			static_assert(std::is_base_of<Component, T>::value);

			for (auto iter = m_components.begin(); iter != m_components.end();)
			{
				auto com = dynamic_cast<T*>(iter->second);
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
			static_assert(std::is_base_of<Component, T>::value);

			for (auto iter = m_components.begin(); iter != m_components.end(); ++iter)
			{
				auto com = dynamic_cast<T*>(*iter);
				if (com)
					return true;
			}
			LOG_Error("No such component at this gameobejct!");
			return false;
		}

	public:
		std::string m_name;
		Transform* m_transform;
	// TODO private
	public:
		std::unordered_map<std::string, Component*> m_components;
	};
}