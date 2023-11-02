#include "GameObject.hpp"
#include "hepch.hpp"

namespace HAIEngine
{

	HAIEngine::GameObject::GameObject()
	{
		m_guid = GenerateGUID(this);
		m_name = "GameObject";
		m_transform = new Transform();
		m_transform->m_parent = this;
		m_components.insert({ m_transform->m_typeName, m_transform });
	}

	HAIEngine::GameObject::~GameObject()
	{
		for (auto iter = m_components.begin(); iter != m_components.end(); ++iter)
		{
			delete iter->second;
			iter = m_components.erase(iter);
		}
	}

	void HAIEngine::GameObject::Update()
	{
		for (const auto& component : m_components)
			component.second->Update();
	}

	json HAIEngine::GameObject::Serialize(const std::string& name)
	{
		json resjson;
		resjson["name"] = name;
		resjson["components"] = json::array();

		for (auto iter = m_components.begin(); iter != m_components.end(); ++iter)
		{
			resjson["components"].emplace_back(iter->second->Serialize(iter->second->m_typeName));
		}

		return resjson;
	}

	void HAIEngine::GameObject::DeSerialize(const json& data)
	{
		// todo

	}

	void HAIEngine::GameObject::AddComponent(Component* component)
	{
		if (m_components.find(component->m_typeName) != m_components.end())
		{
			LOG_Error("Gameobject had a same type component, can not add again!");
			return;
		}
		m_components.insert({ component->m_typeName, component });
	}

	template <class T>
	T* HAIEngine::GameObject::GetComponent()
	{
		for (auto iter = m_components.begin(); iter != m_components.end(); iter++)
		{
			auto com = dynamic_cast<T*>(*iter);
			if (com)
			{
				return com;
			}
		}
		LOG_Error("No such component!");
		return nullptr;
	}

	template <class T>
	void HAIEngine::GameObject::RemoveComponent()
	{
		for (auto iter = m_components.begin(); iter != m_components.end(); iter++)
		{
			auto com = dynamic_cast<T*>(*iter);
			if (com)
			{
				return com;
			}
		}
		LOG_Error("No such component at this gameobejct!");
		return nullptr;
	}
}
