#include "Reflection.hpp"
#include "ECS/Component.hpp"
#include "ECS/Components/Camera.hpp"
#include "ECS/Components/Light.hpp"
#include "ECS/Components/MeshFilter.hpp"
#include "ECS/Components/MeshRenderer.hpp"
#include "ECS/Components/Transform.hpp"
#include "ECS/GameObject.hpp"

namespace HAIEngine
{
	REFLECTION(GameObject, GameObject);
	REFLECTION(Transform, Component);
	REFLECTION(Light, Component);
	REFLECTION(Camera, Component);

	void* ReflectionManager::CreateClassByName(std::string className)
	{
		auto iter = classMap.find(className);
		if (iter != classMap.end())
			return iter->second->createFunc();

		return nullptr;
	}

	std::shared_ptr<void> ReflectionManager::GetInstanceByName(std::string className)
	{
		auto iter = classMap.find(className);
		if (iter != classMap.end())
			return iter->second->getInstance();

		return nullptr;
	}

	void ReflectionManager::RegistClass(std::string name, Reflection* method)
	{
		members.push_back(name.c_str());
		memberByTag.insert(std::pair<ReflectionTag, std::string>(method->tag, method->className));
		classMap.insert(std::pair<std::string, Reflection*>(name, method));
	}
}
