#include "GameObject.hpp"
#include "hepch.hpp"
#include "Components/Transform.hpp"
#include "Core/Reflection.hpp"

namespace HAIEngine
{
	REFLECTION(GameObject, GameObject);

	HAIEngine::GameObject::GameObject()
		: m_name("GameObject")
	{
		m_transform = new Transform();
		AddComponent(m_transform);
	}

	HAIEngine::GameObject::GameObject(std::string name)
		: m_name(name)
	{
		m_transform = new Transform();
		AddComponent(m_transform);
	}

	HAIEngine::GameObject::GameObject(std::string name, size_t guid)
		: m_name(name)
	{
		m_guid = guid;
		m_transform = new Transform();
		AddComponent(m_transform);
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
		resjson["guid"] = m_guid;
		resjson["components"] = json::array();

		for (auto iter = m_components.begin(); iter != m_components.end(); ++iter)
		{
			resjson["components"].emplace_back(iter->second->Serialize(iter->second->m_typeName));
		}

		return resjson;
	}

	void HAIEngine::GameObject::DeSerialize(const json& data)
	{
		m_name = data["name"].get<std::string>();
		m_guid = data["guid"].get<size_t>();

		auto& componentsData = data["components"];
		for (int i = 0; i < componentsData.size(); ++i)
		{
			auto& componentData = componentsData[i];
			// transform init longly
			if (componentData["type"].get<std::string>() == "Transform")
			{
				m_transform->DeSerialize(componentData);
				continue;
			}
			auto gComponent = static_cast<Component*>(HAIEngine::ReflectionManager::GetInstance().CreateClassByName
				(componentData["type"].get<std::string>()));
			
			gComponent->DeSerialize(componentData);

			AddComponent(gComponent);
		}

	}

	void HAIEngine::GameObject::AddComponent(Component* component)
	{
		if (m_components.find(component->m_typeName) != m_components.end())
		{
			LOG_Error("Gameobject had a same type component, can not add again!");
			return;
		}
		component->m_fatherGO = this;
		m_components.insert({ component->m_typeName, component });
	}

}
