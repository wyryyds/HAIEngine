#pragma once
#include "Core/ISerialize.hpp"
#include "Component.hpp"
#include "Components/Transform.hpp"

namespace HAIEngine
{
	class GameObject : public ISerializeable
	{
	public:
		GameObject();
		virtual ~GameObject();

		void Update();

		virtual void Serialize(std::string name = "GameObject") override;
		virtual void DeSerialize(const json& data) override;

		void AddComponent(Component* component);

		template <class T>
		T* GetComponent();

		template <class T>
		void RemoveComponent();

		static size_t GenerateGUID(const GameObject* obj) {
			return std::hash<const GameObject*>{}(obj);
		}

		size_t GetGUID() const {
			return m_guid;
		}
	public:
		std::string m_name;
		size_t m_guid;

		Transform* m_transfrom;
	private:
		std::unordered_map<std::string, Component*> m_components;
	};
}