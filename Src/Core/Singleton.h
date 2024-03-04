#pragma once
#include<iostream>

namespace HAIEngine
{
	template<class T>
	class Singleton
	{
	public:
		static T& GetInstance() 
		{
			static T s_instance;
			return s_instance;
		}

		Singleton(const Singleton&) = delete;
		Singleton& operator=(const Singleton&) = delete;

	protected:
		Singleton() {}
	};
}