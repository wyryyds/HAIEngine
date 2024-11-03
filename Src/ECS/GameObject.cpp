#include "GameObject.hpp"
#include "hepch.hpp"
#include "Components/Transform.hpp"
#include "Core/Reflection.hpp"

namespace HAIEngine
{
	//REFLECTION(GameObject, GameObject);

	GameObject::GameObject() : m_name("GameObject")
	{
		m_transform = AddComponent<Transform>();
	}

	GameObject::GameObject(std::string name) : m_name(std::move(name))
	{
		m_transform = AddComponent<Transform>();
	}

	GameObject::GameObject(std::string name, size_t guid) : m_name(std::move(name))
	{
		m_guid = guid;
		m_transform = AddComponent<Transform>();
	}

	GameObject::GameObject(const GameObject& other) : ISerializable(other), m_name(other.m_name)
	{
		m_transform = AddComponent<Transform>(*other.m_transform);
		for(const auto& [compName, component] : other.m_components)
		{
			m_components[compName] = component->Clone();
		}
	}

	GameObject& GameObject::operator=(const GameObject& other)
	{
		if(this != &other)
		{
			m_name = other.m_name;
			m_transform = other.m_transform;
			m_components.clear();
			for (const auto& [compName, component] : other.m_components)
			{
				m_components[compName] = component->Clone();
			}
		}

		return *this;
	}

	GameObject::GameObject(GameObject&& other) noexcept
		: m_name(std::move(other.m_name)),
		  m_transform(other.m_transform),
		  m_components(std::move(other.m_components))
	{
		other.m_transform = nullptr;
		other.m_components.clear();
	}

	GameObject& GameObject::operator=(GameObject&& other) noexcept
	{
		if(this != &other)
		{
			delete m_transform;
			for (auto& [_, component] : m_components)
				component.reset();
			m_components.clear();

			m_name = std::move(other.m_name);
			m_transform = other.m_transform;
			m_components = std::move(other.m_components);

			other.m_transform = nullptr;
			other.m_components.clear();
		}
		
		return *this;
	}

	GameObject::~GameObject()
	{
		m_transform = nullptr;
		m_components.clear();
	}

	void GameObject::Update(TimeStep ts)
	{
		for (const auto& component : m_components)
			component.second->Update(ts);
	}

	json GameObject::Serialize()
	{
		json resJson;
		resJson["name"] = SerializeHelper::SerializeData(m_name);
		resJson["guid"] = SerializeHelper::SerializeData(m_guid);
		resJson["components"] = json::array();

		for (auto& component : m_components)
		{
			resJson["components"].emplace_back(component.second->Serialize());
		}

		return resJson;
	}

	void GameObject::DeSerialize(const json& jsonData)
	{
		m_name = JsonSerializeHelper::DeSerializeData<std::string>(jsonData["name"]);
		m_guid = JsonSerializeHelper::DeSerializeData<size_t>(jsonData["guid"]);

		auto& componentsData = jsonData["components"];
		for (const auto& componentData : componentsData)
		{
			auto typeName = JsonSerializeHelper::DeSerializeData<std::string>(componentData["type"]);
			if (typeName == "Transform")
			{
				m_transform->DeSerialize(componentData);
				continue;
			}
			
			auto component = std::unique_ptr<Component>(static_cast<Component*>(
				HAIEngine::ReflectionManager::GetInstance().CreateClassByName(typeName)));
			component->m_fatherGO = this;
			component->DeSerialize(componentData);
			
			AddComponent(std::move(component));
		}

	}

	// void GameObject::AddComponent(Component* component)
	// {
	// 	if (m_components.contains(component->m_typeName))
	// 	{
	// 		LOG_Error("GameObject had a same type component, can not add again!");
	// 		return;
	// 	}
	// 	component->SetFatherGO(this);
	// 	m_components.insert({ component->m_typeName, component });
	// }

}
