#pragma once
#include "Core/ISerialize.hpp"

namespace HAIEngine
{
	class Component : public ISerializeable
	{
	public:
		Component() : m_guid(GenerateGUID(this)), m_typeName("Base") {};
		virtual ~Component() = default;
		virtual void OnAwake() {}
		virtual void Update() {}
		virtual void OnRemove() {}

		static size_t GenerateGUID(const Component* obj) {
			return std::hash<const Component*>{}(obj);
		}

		const size_t GetGUID() const {
			return m_guid;
		}
		
	public:
		std::string m_typeName;
	protected:
		size_t m_guid;

	};
}