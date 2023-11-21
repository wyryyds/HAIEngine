#include "GameObject.hpp"
#include "hepch.hpp"
#include "Components/Transform.hpp"
#include "Core/Reflection.hpp"

namespace HAIEngine
{
	//REFLECTION(GameObject, GameObject);

	GameObject::GameObject()
		: m_name("GameObject")
	{
		m_transform = new Transform();
		AddComponent(m_transform);
	}

	GameObject::GameObject(std::string name)
		: m_name(name)
	{
		m_transform = new Transform();
		AddComponent(m_transform);
	}

	GameObject::GameObject(std::string name, size_t guid)
		: m_name(name)
	{
		m_guid = guid;
		m_transform = new Transform();
		AddComponent(m_transform);
	}

	GameObject::~GameObject()
	{
		for (auto iter = m_components.begin(); iter != m_components.end(); ++iter)
		{
			delete iter->second;
			iter = m_components.erase(iter);
		}
	}

	void GameObject::Update(TimeStep ts)
	{
		for (const auto& component : m_components)
			component.second->Update(ts);
	}

	json GameObject::Serialize()
	{
		json resjson;
		resjson["name"] = SerializeHelper::SerializeData(m_name);
		resjson["guid"] = SerializeHelper::SerializeData(m_guid);
		resjson["components"] = json::array();

		for (auto iter = m_components.begin(); iter != m_components.end(); ++iter)
		{
			resjson["components"].emplace_back(iter->second->Serialize());
		}

		return resjson;
	}

	void GameObject::DeSerialize(const json& jsonData)
	{
		m_name = SerializeHelper::DeSerializeData<std::string>(jsonData["name"]);
		m_guid = SerializeHelper::DeSerializeData<size_t>(jsonData["guid"]);

		auto& componentsData = jsonData["components"];
		for (int i = 0; i < componentsData.size(); ++i)
		{
			auto& componentData = componentsData[i];
			if (SerializeHelper::DeSerializeData<std::string>(componentData["type"]) == "Transform")
			{
				m_transform->DeSerialize(componentData);
				continue;
			}

			auto gComponent = static_cast<Component*>(HAIEngine::ReflectionManager::GetInstance().CreateClassByName
				(SerializeHelper::DeSerializeData<std::string>(componentData["type"])));			
			gComponent->DeSerialize(componentData);
			AddComponent(gComponent);
		}

	}

	void GameObject::AddComponent(Component* component)
	{
		if (m_components.find(component->m_typeName) != m_components.end())
		{
			LOG_Error("Gameobject had a same type component, can not add again!");
			return;
		}
		component->SetFatherGO(this);
		m_components.insert({ component->m_typeName, component });
	}

}
