#pragma once
#include "Core/Core.hpp"

#include "nlohmann/json.hpp"

namespace HAIEngine
{
	using json = nlohmann::json;

	class HESerializeFile
	{
		// TODO add default path
		const std::string DefaultPath = ASSTESPATH;
	public:
		HESerializeFile(const std::string& filepath);
		~HESerializeFile() = default;
		inline json& GetJsonData(){ return m_jsonData; }
		void Save();

	private:
		std::string m_filepath;
		json m_jsonData;
	};

	class ISerializeable
	{
	public:
		virtual void Serialize(std::string& name) = 0;
		virtual void DeSerialize() = 0;

		virtual json& GetJsonData() = 0;

	private:
		json m_jsonData;
	};
}