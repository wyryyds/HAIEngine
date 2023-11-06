#include "Reflection.hpp"

namespace HAIEngine
{
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
