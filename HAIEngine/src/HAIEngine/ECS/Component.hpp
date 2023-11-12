#pragma once
#include "Core/ISerialize.hpp"
#include "Core/TimeStep.hpp"

namespace HAIEngine
{
	class GameObject;

	class Component : public ISerializeable
	{
	public:
		Component(std::string typeName) : m_typeName(typeName) {};
		virtual ~Component() = default;
		virtual void OnAwake() {}
		virtual void Update(TimeStep ts) {}
		virtual void OnRemove() {}
		
	public:
		std::string m_typeName;
		GameObject* m_fatherGO;
	};
}