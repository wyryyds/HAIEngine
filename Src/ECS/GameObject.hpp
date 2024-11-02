#pragma once
#include "Core/ISerialize.hpp"
#include "Component.hpp"
#include "Core/Log.hpp"

namespace HAIEngine
{
	class Transform;
	class TimeStep;

	class GameObject : public ISerializable
	{
	public:
		GameObject();
		GameObject(std::string name);
		GameObject(std::string name, size_t guid);
		GameObject(const GameObject& other);
		GameObject& operator=(const GameObject& other);
		GameObject(GameObject&& other) noexcept;
		GameObject& operator=(GameObject&& other) noexcept;
		~GameObject() override;

		void Update(TimeStep ts);

		virtual json Serialize() override;
		virtual void DeSerialize(const json& data) override;

		inline Transform& GetTransform() const { return *m_transform; }
		// void AddComponent(Component* component);

		template<typename T, typename... Args>
		T* AddComponent(Args&&... args)
		{
			static_assert(std::is_base_of_v<Component, T>, "T must inherit from Component");
        
			auto component = std::make_unique<T>(std::forward<Args>(args)...);
			T* rawPtr = component.get();
			m_components[component->m_typeName] = std::move(component);
			return rawPtr;
		}

		template<typename T>
		T* AddComponent(std::unique_ptr<T> component)
		{
			static_assert(std::is_base_of_v<Component, T>, "T must inherit from Component");
			T* rawPtr = component.get();
			m_components[component->m_typeName] = std::move(component);
			return rawPtr;
		}
		

		// template <class T>
		// T& GetComponent()
		// {
		// 	static_assert(std::is_base_of_v<Component, T>, "T must be derived from Component");
		//
		// 	for (const auto& pair : m_components)
		// 	{
		// 		auto com = dynamic_cast<T*>(pair.second);
		// 		if (com)
		// 		{
		// 			return *com;
		// 		}
		// 	}
		//
		// 	LOG_Error("No such component at this gameObject!");
		// 	static T invalidComponent;
		// 	return invalidComponent;
		// }

		template<typename T>
		std::optional<std::reference_wrapper<T>> GetComponent()
		{
			static_assert(std::is_base_of_v<Component, T>, "T must be derived from Component");
			
			if (const auto it = m_components.find(typeid(T).name()); it != m_components.end())
			{
				if (auto* derived = dynamic_cast<T*>(it->second)) 
					return std::ref(*derived);
			}
			return std::nullopt;
		}

		template <class T>
		bool RemoveComponent()
		{
			static_assert(std::is_base_of_v<Component, T>(), "T must be derived from Component");

			for (auto iter = m_components.begin(); iter != m_components.end();)
			{
				if (auto com = dynamic_cast<T*>(iter->second))
				{
					m_components.erase(iter);
					return true;
				}
				else
				{
					++iter;
				}
			}
			LOG_Error("No such component at this gameObject!");
			return false;
		}

		template <class T>
		bool HasComponent()
		{
			static_assert(std::is_base_of_v<Component, T>(), "T must be derived from Component");

			for (auto& m_component : m_components)
			{
				if (auto com = dynamic_cast<T*>(m_component))
					return true;
			}
			LOG_Error("No such component at this gameObject!");
			return false;
		}

	public:
		std::string m_name;
		Transform* m_transform;
	// TODO private
	public:
		std::unordered_map<std::string, std::unique_ptr<Component> > m_components;
	};
}