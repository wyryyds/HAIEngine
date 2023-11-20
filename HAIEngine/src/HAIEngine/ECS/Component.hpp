#pragma once
#include "Core/ISerialize.hpp"
#include "Core/TimeStep.hpp"

namespace HAIEngine
{
	class GameObject;
	class Transform;
	class Component : public ISerializeable
	{
	public:
		Component(std::string typeName) : m_typeName(typeName) {};
		Component(std::string typeName, GameObject* father);
		virtual ~Component() = default;
		virtual void OnAwake() {}
		virtual void Update(TimeStep ts) {}
		virtual void OnRemove() {}
		
		Transform* GetTransform() const;
		void SetFatherGO(GameObject* go);
		inline GameObject* GetFatherGO() const { return m_fatherGO; }
	public:
		std::string m_typeName = "Component";
		GameObject* m_fatherGO = nullptr;
	};
}