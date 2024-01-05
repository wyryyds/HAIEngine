#pragma once
#include "Core/ISerialize.hpp"
#include "Editor/OnGui.hpp"

#include "Core/TimeStep.hpp"

namespace HAIEngine
{
	class GameObject;
	class Transform;

	class Component : public ISerializeable, public IGuiDisplay
	{
	public:
		Component(std::string typeName) : m_typeName(typeName) {};
		Component(std::string typeName, GameObject* father);
		virtual ~Component() = default;
		virtual void OnAwake() {}
		virtual void Update(TimeStep ts) {}
		virtual void OnRemove() {}
		
		Transform& GetTransform() const;
		inline GameObject& GetFatherGO() const { return *m_fatherGO; }

		void SetFatherGO(GameObject* go);

	public:
		std::string m_typeName = "Component";
		GameObject* m_fatherGO = nullptr;
	};
}