#pragma once
#include "Core/ISerialize.hpp"

namespace HAIEngine
{
	class Component : public ISerializeable
	{
	public:
		Component() : m_guid(GenerateGUID(this)) {};
		virtual ~Component() = default;
		virtual void OnAwake() {}
		virtual void Update() {}
		virtual void OnRemove() {}

		static size_t GenerateGUID(const Component* obj) {
			return std::hash<const Component*>{}(obj);
		}

		size_t GetGUID() const {
			return m_guid;
		}
		// TODO OnGUI
	private:
		size_t m_guid;

	};
}