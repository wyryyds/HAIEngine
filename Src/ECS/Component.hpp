#pragma once
#include "Core/ISerialize.hpp"
#include "Editor/OnGui.hpp"

#include "Core/TimeStep.hpp"

namespace HAIEngine
{
	class GameObject;
	class Transform;

	class Component : public ISerializable, public IGuiDisplay
	{
	public:
		Component(std::string typeName) : m_typeName(std::move(typeName)) {}
		Component(std::string typeName, GameObject* father);
		Component(const Component& other) = default;
		Component& operator=(const Component& other) = default;
		Component(Component&& other) = default;
		Component& operator=(Component&& other) = default;
		virtual ~Component() override = default;
		virtual std::unique_ptr<Component> Clone() const { return std::make_unique<Component>(*this); }
		virtual void OnAwake() {}
		virtual void Update(TimeStep ts) {}
		virtual void OnRemove() {}
		virtual json Serialize() override;
		virtual void DeSerialize(const json& jsonData) override {}
		virtual void GuiDisplay() override {}
		
		Transform& GetTransform() const;
		inline GameObject& GetFatherGO() const { return *m_fatherGO; }

		void SetFatherGO(GameObject* go);

	public:
		std::string m_typeName = "Component";
		GameObject* m_fatherGO = nullptr;
	};
}