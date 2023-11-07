#pragma once
#include "Core/ISerialize.hpp"

namespace HAIEngine
{
	class Component : public ISerializeable
	{
	public:
		Component() : m_typeName("Component") {};
		virtual ~Component() = default;
		virtual void OnAwake() {}
		virtual void Update() {}
		virtual void OnRemove() {}
		
	public:
		std::string m_typeName;

	};
}