#pragma once
#include "Core/Core.hpp"

#include "nlohmann/json.hpp"

namespace HAIEngine
{
	using json = nlohmann::ordered_json;

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
		virtual void Serialize(std::string name = "Default") = 0;
		virtual void DeSerialize(const json& jsondata) = 0;

		inline virtual json& GetJsonData() { return m_jsonData; }

	protected:
		json m_jsonData;
	};
}