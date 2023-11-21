#pragma once
#include "Singleton.h"

#include <map>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

namespace HAIEngine
{
	using CreateObjectFunc = void* (*)(void);
	using GetInstanceFunc = std::shared_ptr<void>(*)(void);

#define REFLECTION(className,tag)													\
	className *Creator##className()													\
	{																				\
		return new className;														\
	}																				\
	RegisterAction registerAction##className(										\
		#className,new Reflection(#className,(CreateObjectFunc)Creator##className,ReflectionTag::##tag))

#define REFLECTIONINSTANCE(className,tag)											\
	className *Creator##className()													\
	{																				\
		return new className;														\
	}																				\
	std::shared_ptr<void> Instance##className()										\
	{																				\
		static std::shared_ptr<void> instance;										\
		if (!instance)																\
			instance = std::shared_ptr<className>(new className());					\
		return instance;															\
	}																				\
	RegisterAction registerAction##className(										\
		#className,new Reflection(#className,(CreateObjectFunc)Creator##className,ReflectionTag::##tag,Instance##className))

	enum class ReflectionTag
	{
		Component,
		GameObject,
		//Material
	};

	struct Reflection
	{
		std::string className;
		CreateObjectFunc createFunc;
		GetInstanceFunc getInstance;
		ReflectionTag tag;

		Reflection(std::string name, CreateObjectFunc func, ReflectionTag _tag, GetInstanceFunc instance = nullptr)
			:className(name), createFunc(func), tag(_tag), getInstance(instance)
		{
		}
	};

	class ReflectionManager : public Singleton<ReflectionManager>
	{
	public:
		ReflectionManager() = default;
		void* CreateClassByName(std::string className);
		std::shared_ptr<void> GetInstanceByName(std::string className);
		const std::vector<std::string>& GetAllMember() const { return members; }
		void RegistClass(std::string name, Reflection* method);

		const std::multimap<ReflectionTag, std::string>::iterator GetMemberStartByTag(ReflectionTag tag) { return memberByTag.find(tag); }
		const int GetMemberCountByTag(ReflectionTag tag)const { return memberByTag.count(tag); }

	private:
		std::unordered_map<std::string, Reflection*> classMap;
		std::vector<std::string> members;
		std::multimap<ReflectionTag, std::string> memberByTag;
	};

	class RegisterAction
	{
	public:
		RegisterAction(std::string className, Reflection* ptrCreateFn)
		{
			ReflectionManager::GetInstance().RegistClass(className, ptrCreateFn);
		}
	};
}